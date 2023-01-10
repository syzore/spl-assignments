#pragma once
#include "../include/User.h"

User::User(std::string name) : name(name), subscriptionsMap(), mIsConnected(false)
{
}

User::~User()
{
    if (subscriptionsMap)
    {
        delete subscriptionsMap;
        subscriptionsMap = nullptr;
    }
}

const string User::getName()
{
    return name;
}

std::map<string, int> *User::getSubscriptionsMap()
{
    return subscriptionsMap;
}

void User::connect()
{
    mIsConnected = true;
}

bool User::isConnected()
{
    return mIsConnected;
}