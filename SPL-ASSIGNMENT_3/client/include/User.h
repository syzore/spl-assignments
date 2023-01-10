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
    User(string name);
    ~User();
    void connect();
    bool isConnected();
    const string getName();
    std::map<string, int> *getSubscriptionsMap();
};