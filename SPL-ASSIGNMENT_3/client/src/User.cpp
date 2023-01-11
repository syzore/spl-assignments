#pragma once
#include "../include/User.h"

User::User() : name(), subscriptionsMap(new std::map<string, int>()), mIsConnected(false)
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

void User::setName(string name)
{
    this->name = name;
}

std::map<string, int> *User::getSubscriptionsMap()
{
    return subscriptionsMap;
}

void User::connect()
{
    mIsConnected = true;
}

void User::disconnect()
{
    mIsConnected = false;
}

const bool User::isConnected() const
{
    return mIsConnected;
}