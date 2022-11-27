#pragma once

#include <vector>

class Simulation;
class Party;
class Agent;

class JoinPolicy
{
public:
    JoinPolicy() = default;
    virtual ~JoinPolicy(){};
    virtual void join(Party &p, Simulation &s) = 0;
    virtual JoinPolicy *clone() const = 0;
};

class MandatesJoinPolicy : public JoinPolicy
{
public:
    MandatesJoinPolicy();
    ~MandatesJoinPolicy();
    void join(Party &p, Simulation &s);
    virtual MandatesJoinPolicy *clone() const override;
};

class LastOfferJoinPolicy : public JoinPolicy
{
public:
    LastOfferJoinPolicy();
    ~LastOfferJoinPolicy();
    void join(Party &p, Simulation &s);
    virtual LastOfferJoinPolicy *clone() const override;
};