#pragma once

#include <vector>

class Simulation;
class Party;
class Agent;

class JoinPolicy
{
public:
    JoinPolicy() = default;
    virtual void join(Party &p, Simulation &s) = 0;
    virtual void addOffer(Agent &) = 0;
    virtual int getOffersSize() const = 0;
};

class MandatesJoinPolicy : public JoinPolicy
{
public:
    MandatesJoinPolicy();
    void join(Party &p, Simulation &s);
    void addOffer(Agent &);
    int getOffersSize() const;

private:
    std::vector<Agent> offers;
};

class LastOfferJoinPolicy : public JoinPolicy
{
public:
    LastOfferJoinPolicy();
    void join(Party &p, Simulation &s);
    void addOffer(Agent &);
    int getOffersSize() const;

private:
    std::vector<Agent> offers;
};