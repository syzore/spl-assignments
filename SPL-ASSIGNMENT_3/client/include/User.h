#pragma once
#include "../include/StringUtil.h"
#include <map>
using namespace std;

class User
{
private:
    std::map<string, int> *subscriptionsMap;
    string name;

public:
    User(string name);
    ~User();
    const string getName();
    std::map<string, int> *getSubscriptionsMap();
};