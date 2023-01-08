#pragma once
#include <string>
#include <vector>
using std::string;
using std::vector;

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
    const int getId() const;
    void join(Simulation &);
    void suggest(Agent &);
    void printNumberOfOffers() const;
    void addOffer(Agent &);
    vector<int> getMOffers();
    int getOffersSize() const;

private:
    int mId;
    int timer;
    int mMandates;
    string mName;
    State mState;
    vector<int> mOffers;
    JoinPolicy *mJoinPolicy;
};
