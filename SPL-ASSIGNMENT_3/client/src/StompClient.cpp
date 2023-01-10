#include <stdlib.h>
#include <iostream>
#include <string>
#include "../include/ConnectionHandler.h"
#include "../include/Constants.h"
// #include "../include/StringUtil.h"
#include "../include/StompClient.h"
#include "../include/StompProtocol.h"
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

	ConnectionHandler connectionHandler(host, port);
	if (!connectionHandler.connect())
	{
		std::cerr << "Cannot connect to " << host << ":" << port << std::endl;
		return 1;
	}

	StompClient client();
	// StompProtocol proto = ...
	std::thread t1(StompClient::keyboard_handler_task, std::ref(connectionHandler));
	std::thread t2(StompClient::socket_listener_task, std::ref(connectionHandler));

	t1.join();
	t2.join();
	return 0;
}

StompClient::StompClient() : currentUser(nullptr), subscriptionId(0), receiptId(0), waitingForResponse(false) {}

StompClient::~StompClient()
{
	if (currentUser)
	{
		delete currentUser;
		currentUser = nullptr;
	}
}

void StompClient::socket_listener_task(ConnectionHandler &connectionHandler)
{
	while (1)
	{
		std::string answer;
		// Get back an answer: by using the expected number of bytes (len bytes + newline delimiter)
		// We could also use: connectionHandler.getline(answer) and then get the answer without the newline char at the end
		if (!connectionHandler.getFrame(answer))
		{
			std::cout << "Disconnected. Exiting...\n"
					  << std::endl;
			break;
		}

		int len = answer.length();
		// A C string must end with a 0 char delimiter.  When we filled the answer buffer from the socket
		// we filled up to the \n char - we must make sure now that a 0 char is also present. So we truncate last character.
		answer.resize(len - 1);
		std::cout << "Passive Listener Reply: " << answer << " " << len << " bytes " << std::endl;
		if (answer == "bye")
		{
			std::cout << "Exiting...\n"
					  << std::endl;
			break;
		}
		else
		{
			parse_then_handle_response(answer);
		}
	}
}

void StompClient::keyboard_handler_task(ConnectionHandler &connectionHandler)
{
	while (1)
	{
		const short bufsize = 1024;
		char buf[bufsize];
		std::cout << "what is your command?" << std::endl;
		std::cin.getline(buf, bufsize);
		std::string commandLine(buf);
		std::vector<std::string> lineParts = StringUtil::split(commandLine, ' ');

		std::string frame = parse_command_line(lineParts);

		std::cout << "encoded frame = \n"
				  << frame << std::endl;

		if (frame == "")
		{
			// cout incorrect command?
			continue;
		}

		if (!connectionHandler.sendFrame(frame))
		{
			std::cout << "Disconnected. Exiting...\n"
					  << std::endl;
			break;
		}
	}
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
		std::cout << line << std::endl;
		int index = line.find(':');
		std::string key = line.substr(0, index);
		std::string value = line.substr(index + 1, line.length());
		args.insert({std::make_pair(key, value)});
	}

	// BODY
	std::string body;
	while (std::getline(iteratable, line))
	{
		if (line == "\0")
			break;
		body += line;
	}

	handle_response(command, args, body);
}

void StompClient::handle_response(std::string command, std::map<std::string, std::string> args, std::string body)
{
	if (command == CONNECTED)
	{
		if (!currentUser)
		{
			std::cout << "Trying to connect the user but the current user is null" << std::endl;
			return;
		}
		currentUser->connect();
	}
	else if (command == RECEIPT)
	{
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

void StompClient::resetCurrentUser()
{
	if (currentUser)
	{
		delete currentUser;
		currentUser = nullptr;
	}
	subscriptionId = 0;
}

std::string StompClient::parse_command_line(std::vector<std::string> lineParts)
{
	std::string command = lineParts[0];

	if (command == command_login)
	{
		return StompProtocol::handle_login_command(lineParts, currentUser);
	}
	else if (command == command_logout)
	{
		return StompProtocol::handle_logout_command(lineParts, currentUser, getNextReceiptId());
	}
	else if (command == command_join)
	{
		return StompProtocol::handle_join_command(lineParts, currentUser, getNextSubscriptionId(), getNextReceiptId());
	}
	else if (command == command_exit)
	{
		return StompProtocol::handle_exit_command(lineParts, currentUser);
	}
	else if (command == command_summary)
	{
		return StompProtocol::handle_summary_command(lineParts, currentUser);
	}
	else
	{
		std::cout << "no command corresponding with " << command << " was found..." << std::endl;
		return "";
	}
}

User *StompClient::getCurrentUser()
{
	return currentUser;
}
