#pragma once
#include "../include/User.h"

User::User(std::string name)
{
    name = name;;
    subscriptionsMap = std::map<string, int>();
}

const string User::getName(){
    return name;
}

std::map<string, int> User::getSubscriptionsMap(){
    return subscriptionsMap;
}