#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <string>
#include <set>

#include "../include/ConnectionHandler.h"
#include "../include/Constants.h"
#include "../include/StompClient.h"
#include "../include/StompProtocol.h"
#include "../include/threadTasks.h"
#include "../include/event.h"
#include "../include/Game.h"

using namespace std;

int main(int argc, char *argv[])
{

	if (argc < 3)
	{
		std::cerr << "Usage: " << argv[0] << " host port" << std::endl
				  << std::endl;
		return -1;
	}

	std::string host = argv[1];
	short port = atoi(argv[2]);

	StompClient client(host, port);

	std::thread t1(keyboard_handler_task, std::ref(client));
	std::thread t2(socket_listener_task, std::ref(client));

	t1.join();
	t2.join();
	return 0;
}

StompClient::StompClient(std::string _host, short _port) : host(_host), port(_port), currentUser(new User()),
														   connectionHandler(new ConnectionHandler(_host, _port)),
														   subscriptionId(0), receiptId(0), mShouldListen(false),
														   userGamesMap() {}

StompClient::~StompClient()
{
	if (currentUser)
	{
		delete currentUser;
		currentUser = nullptr;
	}

	if (connectionHandler)
	{
		delete connectionHandler;
		connectionHandler = nullptr;
	}
}

void socket_listener_task(StompClient &client)
{
	ConnectionHandler *connectionHandler = client.getConnectionHandler();

	while (1)
	{
		while (!connectionHandler->isConnected() || !client.shouldListen())
		{
			// wait..
		}

		std::string answer;
		if (!connectionHandler->getFrame(answer))
		{
			std::cout << "Disconnected. Exiting...\n"
					  << std::endl;
			client.closeConnection();
			continue;
		}

		answer.resize(answer.length() - 1);
		std::cout << "Frame from server: " << answer << std::endl;

		client.parse_then_handle_response(answer);
	}
}

void keyboard_handler_task(StompClient &client)
{
	ConnectionHandler *connectionHandler = client.getConnectionHandler();

	while (1)
	{
		const short bufsize = 1024;
		char buf[bufsize];
		std::cout << "What is your command?" << std::endl;
		std::cin.getline(buf, bufsize);
		std::string commandLine(buf);
		std::vector<std::string> lineParts = StringUtil::split(commandLine, ' ');

		std::string frame = client.parse_command_line(lineParts);

		if (frame == EMPTY_BODY)
		{
			client.removeLastCommand();

			continue;
		}

		if (!connectionHandler->sendFrame(frame))
		{
			std::cout << "Disconnected. Exiting...\n"
					  << std::endl;
			client.closeConnection();
			break;
		}

		std::queue<std::string> *framesQueue = client.getCurrentUser()->getEventsReportQueue();
		while (framesQueue->size() > 0)
		{
			frame = framesQueue->front();
			framesQueue->pop();

			if (!connectionHandler->sendFrame(frame))
			{
				std::cout << "Disconnected. Exiting...\n"
						  << std::endl;
				client.closeConnection();
				break;
			}
		}
	}
}

const bool StompClient::shouldListen() const
{
	return mShouldListen;
}

void StompClient::setShouldListen(bool shouldListen)
{
	mShouldListen = shouldListen;
}

ConnectionHandler *StompClient::getConnectionHandler()
{
	return connectionHandler;
}

void StompClient::setConnectionHandler(ConnectionHandler *ch)
{
	connectionHandler = ch;
}

void StompClient::parse_then_handle_response(std::string answer)
{
	std::istringstream iteratable(answer);
	std::string line;
	std::getline(iteratable, line);

	// COMMAND
	std::string command = std::string(line);

	// ARGUMENTS
	std::map<std::string, std::string> args;
	while (std::getline(iteratable, line))
	{
		if (line.empty())
			break;
		int index = line.find(':');
		std::string key = line.substr(0, index);
		std::string value = line.substr(index + 1, line.length());
		args.insert(std::make_pair(key, value));
	}

	// BODY
	std::string body;
	while (std::getline(iteratable, line))
	{
		body += line + "\n";
	}

	handle_response(command, args, body);
}

void StompClient::handle_response(std::string command, std::map<std::string, std::string> args, std::string body)
{

	std::string lastCommand;

	if (command == MESSAGE)
	{
		lastCommand = peekAtLastCommand();
	}
	else
	{
		lastCommand = removeLastCommand();
	}

	if (command == CONNECTED)
	{
		currentUser->connect();
	}
	else if (command == RECEIPT && lastCommand == command_logout)
	{
		closeConnection();
	}
	else if (command == MESSAGE)
	{
		handleReceivedReport(body);
	}
	else if (command == ERROR)
	{
		closeConnection();
	}
}

void StompClient::handleReceivedReport(std::string report_body)
{
	int first_index = report_body.find_first_of(': ') + 1;
	int second_index = report_body.find_first_of('\n');
	std::string name = report_body.substr(first_index, second_index - first_index);

	Event event = parseEventString(report_body);
	std::string game_name = event.get_game_name();
	if (userGamesMap.find(name) == userGamesMap.end())
	{
		// if the user is not in the map
		Game game(game_name);
		game.addEvent(event);
		userGamesMap.insert({name, {game}});
	}
	else
	{
		std::vector<Game> games = userGamesMap[name];
		auto it = find_if(games.begin(), games.end(), [&game_name](const Game &obj)
						  { return obj.getGameName() == game_name; });

		if (it != games.end())
		{
			// the game is in the user's vector
			auto index = std::distance(games.begin(), it);
			userGamesMap[name].at(index).addEvent(event);
		}
		else
		{
			// the game is NOT in the user's vector
			Game game(game_name);
			game.addEvent(event);
			userGamesMap[name].insert(userGamesMap[name].begin(), game);
		}
	}
}

const int StompClient::getNextSubscriptionId()
{
	int output = subscriptionId;
	subscriptionId++;
	return output;
}

const int StompClient::getNextReceiptId()
{
	int output = receiptId;
	receiptId++;
	return output;
}

std::string StompClient::parse_command_line(std::vector<std::string> lineParts)
{
	if (lineParts.size() == 0)
		return EMPTY_BODY;

	std::string command = lineParts[0];

	lastCommandsQueue.push(command);

	if (command == command_login)
	{
		setShouldListen(true);
		std::string address = lineParts[1];
		std::string login = lineParts[2];
		std::string passcode = lineParts[3];
		return StompProtocol::handle_login_command(login, address, passcode, currentUser, getConnectionHandler());
	}
	else if (command == command_logout)
	{
		return StompProtocol::handle_logout_command(currentUser, getNextReceiptId());
	}
	else if (command == command_join)
	{
		std::string destination = lineParts[1];
		return StompProtocol::handle_join_command(destination, currentUser, getNextSubscriptionId(), getNextReceiptId());
	}
	else if (command == command_exit)
	{
		std::string destination = lineParts[1];
		return StompProtocol::handle_exit_command(currentUser, destination, getNextSubscriptionId(), getNextReceiptId());
	}
	else if (command == command_summary)
	{
		std::string game_name = lineParts[1];
		std::string user_name = lineParts[2];
		std::string file_name = lineParts[3];
		handle_summary_command(game_name, user_name, file_name);
		return EMPTY_BODY;
	}
	else if (command == command_report)
	{
		removeLastCommand(); // in case of REPORT SEND command we are not interested to know the last command since a user can receieve the MESSAGE without sending the SEND beforehand.
		std::string file = lineParts[1];
		names_and_events names_and_events_ = parseEventsFile(file);
		return StompProtocol::handle_report_command(currentUser, names_and_events_);
	}
	else
	{
		std::cout << "no command corresponding with " << command << " was found..." << std::endl;
		return EMPTY_BODY;
	}
}

User *StompClient::getCurrentUser()
{
	return currentUser;
}

/// @brief will print the game updates it got from {user_name} for {game_name} into the provided {file_name}.
/// @param game_name name of the game we need to save to file.
/// @param user_name nave of the user holding the game summary.
/// @param file_name is the path to the file name we need to save the game summary to.
void StompClient::handle_summary_command(std::string game_name, std::string user_name, std::string file_name)
{
	try
	{
		std::vector<Game> user_saved_games = userGamesMap.at(user_name);
		for (Game game : user_saved_games)
		{
			if (game.getGameName() == game_name)
			{
				fstream out;
				out.open(file_name, ios::out | ios::trunc);

				out << game.toString();

				out.close();

				return;
			}
		}

		std::cerr << "requested game " << game_name << " was not found for this user.." << '\n';
	}
	catch (const std::exception &e)
	{
		std::cerr << "requested user " << user_name << " is not found in our databases.." << '\n';
		std::cerr << e.what() << '\n';
		return;
	}
}

void StompClient::closeConnection()
{
	setShouldListen(false);
	currentUser->disconnect();
	connectionHandler->close();
	subscriptionId = 0;
	lastCommandsQueue.empty();
}

std::string StompClient::peekAtLastCommand()
{
	if (lastCommandsQueue.empty())
		return EMPTY_BODY;

	return lastCommandsQueue.front();
}

std::string StompClient::removeLastCommand()
{
	if (lastCommandsQueue.empty())
		return EMPTY_BODY;

	std::string lastCommand = lastCommandsQueue.front();
	lastCommandsQueue.pop();
	return lastCommand;
}