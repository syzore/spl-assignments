#include <stdlib.h>
#include <iostream>
#include <string>
#include "../include/ConnectionHandler.h"
#include "../include/Constants.h"
// #include "../include/StringUtil.h"
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

	std::thread t1(StompProtocol::keyboard_handler_task, std::ref(connectionHandler));
	std::thread t2(StompProtocol::socket_listener_task, std::ref(connectionHandler));

	t1.join();
	t2.join();
	return 0;
}

void StompProtocol::socket_listener_task(ConnectionHandler &connectionHandler)
{
	while (1)
	{
		const short bufsize = 1024;
		char buf[bufsize];
		std::string answer;
		// Get back an answer: by using the expected number of bytes (len bytes + newline delimiter)
		// We could also use: connectionHandler.getline(answer) and then get the answer without the newline char at the end
		if (!connectionHandler.getLine(answer))
		{
			std::cout << "Disconnected. Exiting...\n"
					  << std::endl;
			break;
		}

		int len = answer.length();
		// A C string must end with a 0 char delimiter.  When we filled the answer buffer from the socket
		// we filled up to the \n char - we must make sure now that a 0 char is also present. So we truncate last character.
		answer.resize(len - 1);
		std::cout << "Passive Listener Reply: " << answer << " " << len << " bytes " << std::endl
				  << std::endl;
		if (answer == "bye")
		{
			std::cout << "Exiting...\n"
					  << std::endl;
			break;
		}
		else
		{
			handle_message_from_subscription(answer);
		}
	}
}

void StompProtocol::handle_message_from_subscription(std::string answer)
{
	std::cout << "got new message from server: " << answer << std::endl;
}

void StompProtocol::keyboard_handler_task(ConnectionHandler &connectionHandler)
{
	while (1)
	{
		const short bufsize = 1024;
		char buf[bufsize];
		std::cout << "what is your commant?" << std::endl;
		std::cin.getline(buf, bufsize);
		std::string command(buf);

		std::string encodedCommand = create_command_frame(command);

		std::cout << "encoded comman = " << encodedCommand << std::endl;

		if (encodedCommand != "")
			if (!connectionHandler.sendFrame(encodedCommand))
			{
				std::cout << "Disconnected. Exiting...\n"
						  << std::endl;
				break;
			}

		// connectionHandler.sendLine(line) appends '\n' to the message. Therefor we send len+1 bytes.

		// We can use one of three options to read data from the server:
		// 1. Read a fixed number of characters
		// 2. Read a line (up to the newline character using the getline() buffered reader
		// 3. Read up to the null character
		std::string answer;
		// Get back an answer: by using the expected number of bytes (len bytes + newline delimiter)
		// We could also use: connectionHandler.getline(answer) and then get the answer without the newline char at the end
		std::cout << "waiting for server response" << std::endl;
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
		std::cout << "Reply: " << answer << " , length = " << len << " bytes " << std::endl;
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

std::string StompProtocol::create_command_frame(std::string line)
{
	std::vector<std::string> lineParts;
	std::string command;
	if (line.find(' ') < line.length())
	{
		lineParts = StringUtil::split(line, ' ');
		std::cout << 'line parts size = ' << lineParts.size() << std::endl;
		command = lineParts[0];
	}
	else
	{
		command = line;
	}

	if (command == command_login)
	{
		return handle_login_command(lineParts);
	}
	else if (command == command_logout)
	{
		return handle_logout_command(lineParts);
	}
	else if (command == command_join)
	{
		return handle_join_command(lineParts);
	}
	else if (command == command_exit)
	{
		return handle_exit_command(lineParts);
	}
	else if (command == command_summary)
	{
		return handle_summary_command(lineParts);
	}
	else
	{
		std::cout << 'no command corresponding with ' << command << " was found..." << std::endl;
		return "";
	}
}

std::string StompProtocol::handle_login_command(std::vector<std::string> lineParts)
{
	std::string address = lineParts[1];
	std::string login = lineParts[2];
	std::string passcode = lineParts[3];
	std::string host = "stomp.cs.bgu.ac.il";
	std::string accept_version = "1.2";

	std::vector<std::pair<std::string, std::string>> args;

	args.push_back(std::pair<std::string, std::string>(host_key, host));
	args.push_back(std::pair<std::string, std::string>(passcode_key, passcode));
	args.push_back(std::pair<std::string, std::string>(accept_version_key, accept_version));
	args.push_back(std::pair<std::string, std::string>(login_key, login));

	return create_command_frame(CONNECT, args, "");
}

std::string StompProtocol::handle_logout_command(std::vector<std::string> lineParts)
{
}
std::string StompProtocol::handle_join_command(std::vector<std::string> lineParts)
{
}
std::string StompProtocol::handle_exit_command(std::vector<std::string> lineParts)
{
}
std::string StompProtocol::handle_summary_command(std::vector<std::string> lineParts)
{
}

std::string StompProtocol::create_command_frame(std::string command, std::vector<std::pair<std::string, std::string>> args, std::string body)
{
	std::string frame;
	frame.append(command + "\n");
	for (std::pair<std::string, std::string> pair : args)
	{
		frame.append(pair.first + ":" + pair.second + "\n");
	}

	if (body != "")
	{
		frame.append("\n");
		frame.append(body + "\n");
	}
	frame.append("\0");

	return frame;
}

void StompProtocol::parse_then_handle_response(std::string answer)
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

void StompProtocol::handle_response(std::string command, std::map<std::string, std::string> args, std::string body)
{
	if (command == CONNECTED)
	{
	}
}
