#pragma once
#include "../include/Game.h"

using namespace std;

Game::Game(string game_name) : game_name(game_name), general_stats(),
                               first_team_stats(), second_team_stats(), events_map(){};

/// @brief add the event to the map and updates the game stats according to the new updates.
/// @param event event to append.
void Game::addEvent(Event event)
{
    events_map.push_back(event);

    for (auto const &update : event.get_game_updates())
    {
        if (general_stats.find(update.first) == general_stats.end())
        {
            general_stats.insert(update);
        }
        else
        {
            general_stats.at(update.first) = update.second;
        }
    }

    for (auto const &update : event.get_team_a_updates())
    {
        if (first_team_stats.find(update.first) == first_team_stats.end())
        {
            first_team_stats.insert(update);
        }
        else
        {
            first_team_stats.at(update.first) = update.second;
        }
    }

    for (auto const &update : event.get_team_b_updates())
    {
        if (second_team_stats.find(update.first) == second_team_stats.end())
        {
            second_team_stats.insert(update);
        }
        else
        {
            second_team_stats.at(update.first) = update.second;
        }
    }
}

void Game::updateGeneralStats(map<string, string> stats)
{
    for (auto const &stat : stats)
    {
        if (general_stats.find(stat.first) == general_stats.end())
        {
            general_stats.insert(stat);
        }
        else
        {
            general_stats.at(stat.first) = stat.second;
        }
    }
}

void Game::updateFirstTeamStats(map<string, string> stats)
{
    for (auto const &stat : stats)
    {
        if (first_team_stats.find(stat.first) == first_team_stats.end())
        {
            first_team_stats.insert(stat);
        }
        else
        {
            first_team_stats.at(stat.first) = stat.second;
        }
    }
}

void Game::updateSecondStats(map<string, string> stats)
{
    for (auto const &stat : stats)
    {
        if (second_team_stats.find(stat.first) == second_team_stats.end())
        {
            second_team_stats.insert(stat);
        }
        else
        {
            second_team_stats.at(stat.first) = stat.second;
        }
    }
}

const string Game::getGameName() const
{
    return game_name;
}

// string game_name;
// map<string, string> general_stats;
// map<string, string> first_team_stats;
// map<string, string> second_team_stats;

// // each event is a map with <time - event_name , description> format
// vector<Event> events_map;

const string Game::toString() const
{
    string result;

    int index = game_name.find_first_of("_");
    string team_a = game_name.substr(0, index);
    string team_b = game_name.substr(index + 1);
    string game_name_formal = team_a + " vs " + team_b;

    result.append(game_name_formal + "\n");
    result.append("Game stats:\n");
    result.append("General stats:\n");
    for (auto const &stat : general_stats)
    {
        result.append(stat.first + ": " + stat.second + "\n");
    }

    result.append(team_a + " stats:\n");
    for (auto const &stat : first_team_stats)
    {
        result.append(stat.first + ": " + stat.second + "\n");
    }

    result.append(team_b + " stats:\n");
    for (auto const &stat : second_team_stats)
    {
        result.append(stat.first + ": " + stat.second + "\n");
    }

    result.append("Game event reports:\n");

    for (auto const &event : events_map)
    {
        int time = event.get_time();
        result.append(std::to_string(time) + " - " + event.get_name() + ":\n\n");

        result.append(event.get_description() + "\n\n\n");
    } 

    return result;
}