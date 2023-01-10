#include <stdlib.h>
#include <iostream>
#include <string>
#include "../include/ConnectionHandler.h"
#include "../include/Constants.h"
#include "../include/StompProtocol.h"


int main(int argc, char *argv[])
{


}

void StompProtocol::handle_message_from_subscription(std::string answer)
{
	std::cout << "got new message from server: " << answer << std::endl;
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

std::string StompProtocol::handle_join_command(std::vector<std::string> lineParts, User *currentUser, int subscriptionId, int receiptId)
{
	if (currentUser != nullptr){
		cout << "user is already connected" << endl;
		return "";
	}
	
	std::vector<std::pair<std::string, std::string>> args;

	args.push_back(std::pair<std::string, std::string>(destination_key, lineParts[1]));
	args.push_back(std::pair<std::string, std::string>(subscription_id_key, std::to_string(subscriptionId)));
	args.push_back(std::pair<std::string, std::string>(receipt_key, std::to_string(receiptId)));
	return create_command_frame(SUBSCRIBE, args, "");
}

std::string StompProtocol::handle_exit_command(std::vector<std::string> lineParts)
{
}

std::string StompProtocol::handle_summary_command(std::vector<std::string> lineParts)
{
}

std::string StompProtocol::handle_report_command(std::vector<std::string> lineParts)
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
;