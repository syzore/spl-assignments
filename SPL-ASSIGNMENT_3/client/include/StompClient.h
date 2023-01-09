#pragma once

#include "../include/ConnectionHandler.h"
#include "../include/User.h"

class StompClient
{
private:
    int id;
    User currentUser;

public:
    StompClient() = default;
    static void keyboard_handler_task(ConnectionHandler &connectionHandler);
    static void socket_listener_task(ConnectionHandler &connectionHandler);
    const int getNextId();
    std::string parse_command_line(std::vector<std::string> lineParts);
    User getCurrentUser();
};