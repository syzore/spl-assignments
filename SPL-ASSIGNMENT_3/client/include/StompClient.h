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
    const int getNextId();
    const int getNextReceiptId();
    void resetCurrentUser();
    std::string parse_command_line(std::vector<std::string> lineParts);
    User *getCurrentUser();
};