#pragma once

#include "../include/ConnectionHandler.h"
#include "../include/StringUtil.h"
#include "../include/User.h"
#include "../include/event.h"
#include "../include/StompClient.h"

class StompProtocol
{
public:
    static std::string handle_login_command(std::string login, std::string address, std::string passcode, User *currentUser, ConnectionHandler *connectionHandler);
    static std::string handle_logout_command(User *currentUser, int receiptId);
    static std::string handle_join_command(std::string destination, User *currentUser, int subscriptionId, int receiptId);
    static std::string handle_exit_command(User *currentUser, std::string destination, int subscriptionId, int receiptId);
    static std::string handle_report_command(User *currentUser, names_and_events nae);
    static std::string create_command_frame(std::string command, std::map<std::string, std::string> args, std::string body);
};
