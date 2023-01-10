#pragma once

#include "../include/ConnectionHandler.h"
#include "../include/User.h"

class StompClient
{
private:
    int subscriptionId;
    int receiptId;
    User *currentUser;

public:
    StompClient() = default;
    void keyboard_handler_task(ConnectionHandler &connectionHandler);
    void socket_listener_task(ConnectionHandler &connectionHandler);
    void parse_then_handle_response(std::string answer);
    void handle_response(std::string command, std::map<std::string, std::string> args, std::string body);

    const int getNextSubscriptionId();
    const int getNextReceiptId();
    void resetCurrentUser();
    std::string parse_command_line(std::vector<std::string> lineParts);
    User *getCurrentUser();
};