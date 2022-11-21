#pragma once

#include <vector>

class Simulation;
class Party;
class Agent;

class JoinPolicy
{
public:
    JoinPolicy() : offers(){};
    virtual void join(Party &p, Simulation &s) = 0;
    virtual void addOffer(Agent &) = 0;

protected:
    std::vector<Agent> offers;
};

class MandatesJoinPolicy : public JoinPolicy
{
public:
    MandatesJoinPolicy() : JoinPolicy(){};
    void join(Party &p, Simulation &s);
    void addOffer(Agent &);
};

class LastOfferJoinPolicy : public JoinPolicy
{
public:
    LastOfferJoinPolicy() : JoinPolicy(){};
    void join(Party &p, Simulation &s);
    void addOffer(Agent &);
};