#pragma once
#include "../include/StringUtil.h"
#include <map>
#include <queue>
using namespace std;

class User
{
private:
    bool mIsConnected;
    string name;
    map<string, int> *subscriptionsMap;
    queue<string> *eventsReportQueue;

public:
    User();
    ~User();
    void connect();
    void disconnect();
    const bool isConnected() const;
    const string getName();
    void setName(string name);
    const bool isNotSubscribed(std::string game_name) const;
    const bool isSubscribed(std::string game_name) const;
    map<string, int> *getSubscriptionsMap();
    queue<string> *getEventsReportQueue();
};