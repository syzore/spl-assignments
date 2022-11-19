#pragma once

class JoinPolicy
{
public:
    JoinPolicy(){};
    virtual void join(Party &p, Simulation &s) = 0;
    virtual void addOffer(int) = 0;

protected:
    vector<int> offers;
};

class MandatesJoinPolicy : public JoinPolicy
{
public:
    MandatesJoinPolicy() = default;
    void join(Party &p, Simulation &s);
    void addOffer(int);
};

class LastOfferJoinPolicy : public JoinPolicy
{
public:
    LastOfferJoinPolicy() = default;
    void join(Party &p, Simulation &s);
    void addOffer(int);
};