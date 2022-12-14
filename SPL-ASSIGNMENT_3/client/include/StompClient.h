#pragma once

#include "../include/ConnectionHandler.h"
#include "../include/User.h"

#include <queue>

class StompClient
{
private:
    int subscriptionId;
    int receiptId;
    User *currentUser;
    bool mShouldListen;
    ConnectionHandler *connectionHandler;
    std::string host;
    short port;
    std::queue<string> lastCommandsQueue;

public:
    StompClient(std::string host, short port);
    void keyboard_handler_task(ConnectionHandler &connectionHandler);
    void socket_listener_task(ConnectionHandler &connectionHandler);
    void parse_then_handle_response(std::string answer);
    void handle_response(std::string command, std::map<std::string, std::string> args, std::string body);
    const int getNextSubscriptionId();
    const int getNextReceiptId();
    bool shouldListen();
    void setShouldListen(bool shouldListen);
        ConnectionHandler *getConnectionHandler();
    std::string parse_command_line(std::vector<std::string> lineParts);
    User *getCurrentUser();
    void closeConnection();
    std::string getLastCommand();
};