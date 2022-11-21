#pragma once
#include <string>

using std::string;

class Agent;
class JoinPolicy;
class Simulation;

enum State
{
    Waiting,
    CollectingOffers,
    Joined
};

class Party
{
public:
    Party(int id, string name, int mandates, JoinPolicy *); //default constructor
    Party (const Party&) = delete; //copy constructor
    Party(Party&&) = default; //move constructor
    Party& operator=(const Party&) = default; //copy assignment
    Party& operator=(Party&&) = delete; //move assignment
    ~Party();

    State getState() const;
    void setState(State state);
    int getMandates() const;
    void step(Simulation &s);
    const string &getName() const;
    int timer;
    const int getId() const;
    void join(Simulation &);
    void suggest(Agent &);

private:
    int mId;
    string mName;
    int mMandates;
    JoinPolicy *mJoinPolicy;
    State mState;
};
