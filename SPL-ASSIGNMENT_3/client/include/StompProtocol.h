#pragma once

#include "../include/ConnectionHandler.h"
#include "../include/StringUtil.h"

// TODO: implement the STOMP protocol
class StompProtocol
{
public:
    static void handle_message_from_subscription(std::string answer);
    static std::string create_command_frame(std::string line);
    static std::string handle_login_command(std::vector<std::string> lineParts);
    static std::string handle_logout_command(std::vector<std::string> lineParts);
    static std::string handle_join_command(std::vector<std::string> lineParts);
    static std::string handle_exit_command(std::vector<std::string> lineParts);
    static std::string handle_summary_command(std::vector<std::string> lineParts);
    static std::string handle_report_command(std::vector<std::string> lineParts);
    static std::string create_command_frame(std::string command, std::vector<std::pair<std::string, std::string>> args, std::string body);
    static void parse_then_handle_response(std::string answer);
    static void handle_response(std::string command, std::map<std::string, std::string> args, std::string body);
    

};
