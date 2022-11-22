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
    Party(int id, string name, int mandates, JoinPolicy *); // default constructor
    Party(const Party &);                                   // copy constructor
    Party(Party &&);                                        // move constructor
    Party &operator=(const Party &);                        // copy assignment
    Party &operator=(Party &&);                             // move assignment
    ~Party();                                               // destructor

    State getState() const;
    void setState(State state);
    int getMandates() const;
    void step(Simulation &s);
    const string &getName() const;
    int timer;
    const int getId() const;
    void join(Simulation &);
    void suggest(Agent &);
    void printNumberOfOffers() const;

private:
    int mId;
    string mName;
    int mMandates;
    JoinPolicy *mJoinPolicy;
    State mState;
};
