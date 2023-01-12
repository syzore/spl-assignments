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
    map<string, int> *getSubscriptionsMap();
    queue<string> *getEventsReportQueue();
};