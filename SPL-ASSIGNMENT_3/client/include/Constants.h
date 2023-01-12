#if !defined(CONSTANTS)
#define CONSTANTS 1

#include <string>

const std::string CONNECTED = "CONNECTED";
const std::string MESSAGE = "MESSAGE";
const std::string RECEIPT = "RECEIPT";
const std::string ERROR = "ERROR";
const std::string CONNECT = "CONNECT";
const std::string SEND = "SEND";
const std::string SUBSCRIBE = "SUBSCRIBE";
const std::string UNSUBSCRIBE = "UNSUBSCRIBE";
const std::string DISCONNECT = "DISCONNECT";

const std::string command_login = "login";
const std::string command_join = "join";
const std::string command_exit = "exit";
const std::string command_summary = "summary";
const std::string command_report = "report";
const std::string command_logout = "logout";

const std::string login_key = "login";
const std::string host_key = "host";
const std::string passcode_key = "passcode";
const std::string accept_version_key = "accept-version";
const std::string destination_key = "destination";
const std::string receipt_key = "receipt";
const std::string subscription_id_key = "id";

const std::string EMPTY_BODY = "";

const std::string general_game_updates_key = "general game updates";
const std::string before_halftime_key = "before halftime";
const std::string team_a_updates_key = "team a updates";
const std::string team_b_updates_key = "team b updates";
const std::string description_key = "description";
const std::string event_name_key = "event name";
const std::string team_a_key = "team a";
const std::string team_b_key = "team b";
const std::string active_key = "active";
const std::string user_key = "user";
const std::string time_key = "time";
#endif