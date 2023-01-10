#pragma once
#include "../include/User.h"

User::User(std::string name)
{
    this->name = name;;
    subscriptionsMap = new std::map<string, int>();
}

const string User::getName(){
    return name;
}

std::map<string, int>* User::getSubscriptionsMap(){
    return subscriptionsMap;
}