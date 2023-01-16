#pragma once

#include "../include/ConnectionHandler.h"
#include "../include/User.h"
#include "../include/Game.h"

#include <queue>
#include <map>

class StompClient
{
private:
    int subscriptionId;
    int receiptId;
    short port;
    bool mShouldListen;
    User *currentUser;
    std::string host;
    ConnectionHandler *connectionHandler;
    std::queue<string> lastCommandsQueue;
    std::map<std::string, vector<Game>> userGamesMap;
    void handle_summary_command(std::string game_name, std::string user_name, std::string file_name);

public:
    StompClient(std::string host, short port);
    void keyboard_handler_task(ConnectionHandler &connectionHandler);
    void socket_listener_task(ConnectionHandler &connectionHandler);
    void parse_then_handle_response(std::string answer);
    void handle_response(std::string command, std::map<std::string, std::string> args, std::string body);
    void handleReceivedReport(std::string report_body);
    const int getNextSubscriptionId();
    const int getNextReceiptId();
    bool shouldListen();
    void setShouldListen(bool shouldListen);
    ConnectionHandler *getConnectionHandler();
    std::string parse_command_line(std::vector<std::string> lineParts);
    User *getCurrentUser();
    void closeConnection();
    std::string peekAtLastCommand();
    std::string removeLastCommand();
};