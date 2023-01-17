#pragma once
#include "../include/User.h"
#include <iostream>

using namespace std;

User::User() : mIsConnected(false), name(), subscriptionsMap(new std::map<string, int>()), eventsReportQueue(new queue<string>())
{
}

User::~User()
{
    if (subscriptionsMap)
    {
        delete subscriptionsMap;
        subscriptionsMap = nullptr;
    }

    if (eventsReportQueue)
    {
        delete eventsReportQueue;
        eventsReportQueue = nullptr;
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

std::queue<string> *User::getEventsReportQueue()
{
    return eventsReportQueue;
}

void User::connect()
{
    if (mIsConnected)
    {
        std::cout << "User named " << name << " tried to connect but is already connected." << std::endl;
        return;
    }
    std::cout << "User named " << name << " connected." << std::endl;
    mIsConnected = true;
}

void User::disconnect()
{
    if (!mIsConnected)
    {
        std::cout << "User named " << name << " tried to disconnect but is not connected." << std::endl;
        return;
    }
    std::cout << "User named " << name << " disconnected." << std::endl;
    mIsConnected = false;
    eventsReportQueue->empty();
}

const bool User::isConnected() const
{
    return mIsConnected;
}

const bool User::isSubscribed(std::string game_name) const
{
    return subscriptionsMap->count(game_name) != 0;
}

const bool User::isNotSubscribed(std::string game_name) const
{
    return subscriptionsMap->count(game_name) == 0;
}