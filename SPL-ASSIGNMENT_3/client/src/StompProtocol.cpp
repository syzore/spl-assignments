#include <stdlib.h>
#include <iostream>
#include <string>
#include "../include/ConnectionHandler.h"
#include "../include/Constants.h"
#include "../include/StompProtocol.h"
#include "../include/User.h"
#include "../include/event.h"

#include <boost/lexical_cast.hpp>

/// @brief Creates a new connection handler object with the given address and creates a CONNECT STOMP frame.
/// @param login the username for login.
/// @param address tcp address in ip:port format.
/// @param passcode the user password for login.
/// @param currentUser the user trying to log in.
/// @param connectionHandler the connection handler object used to connect to the server.
/// @return CONNECT STOMP frame or an empty body in case of an error.
std::string StompProtocol::handle_login_command(std::string login, std::string address, std::string passcode, User *currentUser, ConnectionHandler *connectionHandler)
{
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
		return EMPTY_BODY;
	}

	int colonIndex = address.find_first_of(':');
	std::string ip = address.substr(0, colonIndex);
	short port = boost::lexical_cast<short>(address.substr(colonIndex + 1, address.length() - colonIndex));
	currentUser->setName(login);

	std::cout << "Connecting to " << ip << ":" << port << std::endl;
	// auto ch = new ConnectionHandler(ip, port);
	// connectionHandler = ch;

	if (!connectionHandler->connect())
	{
		std::cerr << "Cannot connect to " << ip << ":" << port << std::endl;
		return EMPTY_BODY;
	}
	std::cout << "Connected!!!" << std::endl;

	std::map<std::string, std::string> args;

	args.insert({host_key, host_value});
	args.insert({passcode_key, passcode});
	args.insert({accept_version_key, accept_version_value});
	args.insert({login_key, login});

	return create_command_frame(CONNECT, args, EMPTY_BODY);
}

/// @brief Creates a DISCONNECT STOMP frame.
/// @param currentUser is the current user trying to disconnect.
/// @param receiptId receipt id.
/// @return DISCONNECT STOMP frame or an empty body in case of an error.
std::string StompProtocol::handle_logout_command(User *currentUser, int receiptId)
{
	if (!currentUser->isConnected())
	{
		std::cout << "You must be logged in before logging out..." << std::endl;
		return EMPTY_BODY;
	}

	std::map<std::string, std::string> args;

	args.insert({receipt_key, std::to_string(receiptId)});

	return create_command_frame(DISCONNECT, args, EMPTY_BODY);
}

/// @brief Creates a SUBSCRIBE STOMP frame.
/// @param destination the topic the user is asking to subscribe to.
/// @param currentUser the user asking to subscribe.
/// @param subscriptionId the subscription is associated with the selected topic (unique withing the user contex alone).
/// @param receiptId recepit id.
/// @return SUBSCRIBE STOMP frame or an empty body in case of an error.
std::string StompProtocol::handle_join_command(std::string destination, User *currentUser, int subscriptionId, int receiptId)
{
	if (!currentUser->isConnected())
	{
		std::cout << "You must be logged in before doing anything else..." << std::endl;
		return EMPTY_BODY;
	}

	if (currentUser->isSubscribed(destination))
	{
		std::cout << "You are already subscribed to this topic..." << std::endl;
		return EMPTY_BODY;
	}

	currentUser->getSubscriptionsMap()->insert({destination, subscriptionId});

	std::map<std::string, std::string> args;

	args.insert({destination_key, destination});
	args.insert({subscription_id_key, std::to_string(subscriptionId)});
	args.insert({receipt_key, std::to_string(receiptId)});
	return create_command_frame(SUBSCRIBE, args, EMPTY_BODY);
}

/// @brief Creates a UNSUBSCRIBE STOMP frame.
/// @param currentUser the user asking to unsubscribe.
/// @param subscriptionId the subscription is associated with the selected topic (unique withing the user contex alone).
/// @param receiptId recepit id.
/// @return UNSUBSCRIBE STOMP frame or an empty body in case of an error.
std::string StompProtocol::handle_exit_command(User *currentUser, std::string destination, int subscriptionId, int receiptId)
{
	if (!currentUser->isConnected())
	{
		std::cout << "You must be logged in before doing anything else..." << std::endl;
		return EMPTY_BODY;
	}

	if (currentUser->isNotSubscribed(destination))
	{
		std::cout << "Cannot unsubscribed from " << destination << " because you are not subscribed to this topic\n"
				  << std::endl;

		return EMPTY_BODY;
	}

	currentUser->getSubscriptionsMap()->erase(destination);

	std::map<std::string, std::string>
		args;

	args.insert({receipt_key, std::to_string(receiptId)});
	args.insert({subscription_id_key, std::to_string(subscriptionId)});

	return create_command_frame(UNSUBSCRIBE, args, EMPTY_BODY);
}

/// @brief handls report command, prepares a single or multiple frames in accordance to the given events.
/// @param currentUser current user sending the events.
/// @param names_and_events_ holds the name of the game and the events that the user is wishing to report.
/// @return the first event frame to send.
std::string StompProtocol::handle_report_command(User *currentUser, names_and_events names_and_events_)
{
	if (!currentUser->isConnected())
	{
		std::cout << "You must be logged in before doing anything else..." << std::endl;
		return EMPTY_BODY;
	}

	std::string destination = names_and_events_.team_a_name + "_" + names_and_events_.team_b_name;
	if (currentUser->isNotSubscribed(destination))
	{
		std::cout << "You must be subscribed to this channel inorder to report it..." << std::endl;
		return EMPTY_BODY;
	}

	// Create all report frames.
	std::map<std::string, std::string> args;
	args.insert({destination_key, destination});
	std::vector<Event> events = names_and_events_.events;
	std::queue<std::string> *eventsReportQueue = currentUser->getEventsReportQueue();
	for (Event event : events)
	{
		std::string body = event.eventBodyBuilder(currentUser->getName());
		std::string frame = create_command_frame(SEND, args, body);
		eventsReportQueue->push(frame);
	}

	// Return the first one.
	std::string firstFrame = eventsReportQueue->front();
	eventsReportQueue->pop();
	return firstFrame;
}

/// @brief Creates a STOMP frame from a set of arguments.
/// @param command the main STOMP command.
/// @param args the map of arguments (as key:value pairs).
/// @param body the body of the message.
/// @return a ready STOMP frame.
std::string StompProtocol::create_command_frame(std::string command, std::map<std::string, std::string> args, std::string body)
{
	std::string frame;
	frame.append(command + "\n");
	for (std::pair<std::string, std::string> pair : args)
	{
		frame.append(pair.first + ":" + pair.second + "\n");
	}

	if (body != EMPTY_BODY)
	{
		frame.append("\n");
		frame.append(body);
	}

	return frame;
}