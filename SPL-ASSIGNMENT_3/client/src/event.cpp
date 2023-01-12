#include "../include/event.h"
#include "../include/Constants.h"
#include "../include/json.hpp"
#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <vector>
#include <sstream>
using json = nlohmann::json;

Event::Event(std::string team_a_name, std::string team_b_name, std::string name, int time,
             std::map<std::string, std::string> game_updates, std::map<std::string, std::string> team_a_updates,
             std::map<std::string, std::string> team_b_updates, std::string description)
    : team_a_name(team_a_name), team_b_name(team_b_name), name(name),
      time(time), game_updates(game_updates), team_a_updates(team_a_updates),
      team_b_updates(team_b_updates), description(description)
{
}

Event::~Event()
{
}

const std::string &Event::get_team_a_name() const
{
    return this->team_a_name;
}

const std::string &Event::get_team_b_name() const
{
    return this->team_b_name;
}

const std::string &Event::get_name() const
{
    return this->name;
}

int Event::get_time() const
{
    return this->time;
}

const std::map<std::string, std::string> &Event::get_game_updates() const
{
    return this->game_updates;
}

const std::map<std::string, std::string> &Event::get_team_a_updates() const
{
    return this->team_a_updates;
}

const std::map<std::string, std::string> &Event::get_team_b_updates() const
{
    return this->team_b_updates;
}

const std::string &Event::get_description() const
{
    return this->description;
}

Event::Event(const std::string &frame_body) : team_a_name(""), team_b_name(""), name(""), time(0), game_updates(), team_a_updates(), team_b_updates(), description("")
{
}

std::string Event::eventBodyBuilder(std::string user_name)
{
    std::string body;
    std::vector<std::pair<std::string, std::string>> args;

    args.push_back(std::pair<std::string, std::string>(user_key, user_name));
    args.push_back(std::pair<std::string, std::string>(team_a_key, team_a_name));
    args.push_back(std::pair<std::string, std::string>(team_b_key, team_b_name));
    args.push_back(std::pair<std::string, std::string>(event_name_key, name));
    args.push_back(std::pair<std::string, std::string>(time_key, std::to_string(time)));
    args.push_back(std::pair<std::string, std::string>(general_game_updates_key, EMPTY_BODY));
    for (std::pair<std::string, std::string> game_update : game_updates)
    {
        args.push_back(std::pair<std::string, std::string>(game_update.first, game_update.second));
    }
    for (std::pair<std::string, std::string> team_a_update : team_a_updates)
    {
        args.push_back(std::pair<std::string, std::string>(team_a_update.first, team_a_update.second));
    }
    for (std::pair<std::string, std::string> team_b_update : team_b_updates)
    {
        args.push_back(std::pair<std::string, std::string>(team_b_update.first, team_b_update.second));
    }
    args.push_back(std::pair<std::string, std::string>(description_key, description));

    for (std::pair<std::string, std::string> pair : args)
    {
        body.append(pair.first + ": " + pair.second + "\n");
    }

    return body;
}

names_and_events parseEventsFile(std::string json_path)
{
    std::ifstream f(json_path);
    json data = json::parse(f);

    std::string team_a_name = data["team a"];
    std::string team_b_name = data["team b"];

    // run over all the events and convert them to Event objects
    std::vector<Event> events;
    for (auto &event : data["events"])
    {
        std::string name = event["event name"];
        int time = event["time"];
        std::string description = event["description"];
        std::map<std::string, std::string> game_updates;
        std::map<std::string, std::string> team_a_updates;
        std::map<std::string, std::string> team_b_updates;
        for (auto &update : event["general game updates"].items())
        {
            if (update.value().is_string())
                game_updates[update.key()] = update.value();
            else
                game_updates[update.key()] = update.value().dump();
        }

        for (auto &update : event["team a updates"].items())
        {
            if (update.value().is_string())
                team_a_updates[update.key()] = update.value();
            else
                team_a_updates[update.key()] = update.value().dump();
        }

        for (auto &update : event["team b updates"].items())
        {
            if (update.value().is_string())
                team_b_updates[update.key()] = update.value();
            else
                team_b_updates[update.key()] = update.value().dump();
        }

        events.push_back(Event(team_a_name, team_b_name, name, time, game_updates, team_a_updates, team_b_updates, description));
    }
    names_and_events events_and_names{team_a_name, team_b_name, events};

    return events_and_names;
}