#pragma once

#include "Simulation.h"

class JoinPolicy
{
public:
    JoinPolicy(){};
    virtual void join(Party &p, Simulation &s) = 0;
    virtual void addOffer(Agent &) = 0;

protected:
    vector<Agent &> offers;
};

class MandatesJoinPolicy : public JoinPolicy
{
public:
    MandatesJoinPolicy() = default;
    void join(Party &p, Simulation &s);
    void addOffer(Agent &);
};

class LastOfferJoinPolicy : public JoinPolicy
{
public:
    LastOfferJoinPolicy() = default;
    void join(Party &p, Simulation &s);
    void addOffer(Agent &);
};