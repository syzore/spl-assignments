#pragma once

#include "../include/ConnectionHandler.h"
#include "../include/StringUtil.h"
#include "../include/User.h"
#include "../include/StompClient.h"

// TODO: implement the STOMP protocol
class StompProtocol
{
public:
    static void handle_message_from_subscription(std::string answer);

    static std::string handle_login_command(std::string login, std::string address, std::string passcode, User *currentUser, ConnectionHandler *connectionHandler);
    static std::string handle_logout_command(User *currentUser, int receiptId);
    static std::string handle_join_command(std::string destination, User *currentUser, int subscriptionId, int receiptId);
    static std::string handle_exit_command(User *currentUser, int subscriptionId, int receiptId);
    static std::string handle_summary_command(User *currentUser);
    static std::string handle_report_command(User *currentUser);
    static std::string create_command_frame(std::string command, std::vector<std::pair<std::string, std::string>> args, std::string body);
};
