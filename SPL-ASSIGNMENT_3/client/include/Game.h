#pragma once

#include "../include/event.h"
#include <string>
#include <map>
#include <vector>

using namespace std;

class Game
{
private:
    string game_name;
    map<string, string> general_stats;
    map<string, string> first_team_stats;
    map<string, string> second_team_stats;

    // each event is a map with <time - event_name , description> format
    vector<Event> events_map;

public:
    Game(string game_name);
    void addEvent(Event event);
    void updateGeneralStats(map<string, string> general_stats);
    void updateFirstTeamStats(map<string, string> general_stats);
    void updateSecondStats(map<string, string> general_stats);
    const string toString() const;
    const string getGameName() const;
};