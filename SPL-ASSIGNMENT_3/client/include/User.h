#pragma once
#include "../include/StringUtil.h"
#include <map>
using namespace std;

class User
{
private:
    std::map<string, int> *subscriptionsMap;
    string name;
    bool mIsConnected;

public:
    User();
    ~User();
    void connect();
    void disconnect();
    const bool isConnected() const;
    const string getName();
    void setName(string name);
    map<string, int> *getSubscriptionsMap();
};