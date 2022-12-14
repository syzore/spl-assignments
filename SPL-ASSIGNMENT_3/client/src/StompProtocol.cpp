#include <stdlib.h>
#include <iostream>
#include <string>
#include "../include/ConnectionHandler.h"
#include "../include/Constants.h"
#include "../include/StompProtocol.h"
#include "../include/User.h"

#include <boost/lexical_cast.hpp>

void StompProtocol::handle_message_from_subscription(std::string answer)
{
	std::cout << "got new message from server: " << answer << std::endl;
}

std::string StompProtocol::handle_login_command(std::string login, std::string address, std::string passcode, User *currentUser, ConnectionHandler *connectionHandler)
{
	// re-write this.. if the user is not null or if not connected
	if (currentUser->isConnected())
	{
		if (login == currentUser->getName())
		{
			std::cout << "You are already logged in" << std::endl;
		}
		else
		{
			std::cout << "Another user is already logged in" << std::endl;
		}
		return "";
	}

	std::string host = "stomp.cs.bgu.ac.il";
	std::string accept_version = "1.2";

	int colonIndex = address.find_first_of(':');

	std::string ip = address.substr(0, colonIndex);
	short port = boost::lexical_cast<short>(address.substr(colonIndex + 1, address.length()));
	currentUser->setName(login);

	std::cout << "connecting...." << std::endl;
	if (!connectionHandler->connect())
	{
		std::cerr << "Cannot connect to " << ip << ":" << port << std::endl;
		return "";
	}
	std::cout << "connected!!!" << std::endl;

	std::vector<std::pair<std::string, std::string>> args;

	args.push_back(std::pair<std::string, std::string>(host_key, host));
	args.push_back(std::pair<std::string, std::string>(passcode_key, passcode));
	args.push_back(std::pair<std::string, std::string>(accept_version_key, accept_version));
	args.push_back(std::pair<std::string, std::string>(login_key, login));

	return create_command_frame(CONNECT, args, "");
}

///
std::string StompProtocol::handle_logout_command(User *currentUser, int receiptId)
{
	if (!currentUser->isConnected())
	{
		std::cout << "You must be logged in before doing anything else..." << std::endl;
		return "";
	}

	std::vector<std::pair<std::string, std::string>> args;

	args.push_back(std::pair<std::string, std::string>(receipt_key, std::to_string(receiptId)));

	return create_command_frame(DISCONNECT, args, EMPTY_BODY);
}

std::string StompProtocol::handle_join_command(std::string destination, User *currentUser, int subscriptionId, int receiptId)
{
	if (!currentUser->isConnected())
	{
		std::cout << "You must be logged in before doing anything else..." << std::endl;
		return "";
	}

	std::vector<std::pair<std::string, std::string>> args;

	args.push_back(std::pair<std::string, std::string>(destination_key, destination));
	args.push_back(std::pair<std::string, std::string>(subscription_id_key, std::to_string(subscriptionId)));
	args.push_back(std::pair<std::string, std::string>(receipt_key, std::to_string(receiptId)));
	return create_command_frame(SUBSCRIBE, args, EMPTY_BODY);
}

std::string StompProtocol::handle_exit_command(User *currentUser, int subscriptionId, int receiptId)
{
	if (!currentUser->isConnected())
	{
		std::cout << "You must be logged in before doing anything else..." << std::endl;
		return "";
	}

	std::vector<std::pair<std::string, std::string>> args;

	args.push_back(std::pair<std::string, std::string>(receipt_key, std::to_string(receiptId)));
	args.push_back(std::pair<std::string, std::string>(subscription_id_key, std::to_string(subscriptionId)));

	return create_command_frame(UNSUBSCRIBE, args, EMPTY_BODY);
}

std::string StompProtocol::handle_summary_command(User *currentUser)
{
	if (!currentUser->isConnected())
	{
		std::cout << "You must be logged in before doing anything else..." << std::endl;
		return "";
	}
}

std::string StompProtocol::handle_report_command(User *currentUser)
{
	if (!currentUser->isConnected())
	{
		std::cout << "You must be logged in before doing anything else..." << std::endl;
		return "";
	}
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