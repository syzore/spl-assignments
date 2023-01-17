#if !defined(CONSTANTS)
#define CONSTANTS 1

#include <string>

static const std::string CONNECTED = "CONNECTED";
static const std::string MESSAGE = "MESSAGE";
static const std::string RECEIPT = "RECEIPT";
static const std::string ERROR = "ERROR";
static const std::string CONNECT = "CONNECT";
static const std::string SEND = "SEND";
static const std::string SUBSCRIBE = "SUBSCRIBE";
static const std::string UNSUBSCRIBE = "UNSUBSCRIBE";
static const std::string DISCONNECT = "DISCONNECT";

static const std::string command_login = "login";
static const std::string command_join = "join";
static const std::string command_exit = "exit";
static const std::string command_summary = "summary";
static const std::string command_report = "report";
static const std::string command_logout = "logout";

static const std::string login_key = "login";
static const std::string host_key = "host";
static const std::string passcode_key = "passcode";
static const std::string accept_version_key = "accept-version";
static const std::string destination_key = "destination";
static const std::string receipt_key = "receipt";
static const std::string subscription_id_key = "id";

static const std::string accept_version_value = "1.2";
static const std::string host_value = "stomp.cs.bgu.ac.il";

static const std::string EMPTY_BODY = "";

static const std::string general_game_updates_key = "general game updates";
static const std::string before_halftime_key = "before halftime";
static const std::string team_a_updates_key = "team a updates";
static const std::string team_b_updates_key = "team b updates";
static const std::string description_key = "description";
static const std::string event_name_key = "event name";
static const std::string team_a_key = "team a";
static const std::string team_b_key = "team b";
static const std::string active_key = "active";
static const std::string user_key = "user";
static const std::string time_key = "time";

#endif