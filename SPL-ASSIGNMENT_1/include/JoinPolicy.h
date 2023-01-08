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
    MandatesJoinPolicy();                                      // constructor
    ~MandatesJoinPolicy();                                     // destructor
    MandatesJoinPolicy(const MandatesJoinPolicy &);            // copy constructor
    MandatesJoinPolicy(MandatesJoinPolicy &&);                 // move contructor
    MandatesJoinPolicy &operator=(const MandatesJoinPolicy &); // copy assignment
    MandatesJoinPolicy &operator=(MandatesJoinPolicy &&);      // move assignment

    void join(Party &p, Simulation &s);
    virtual MandatesJoinPolicy *clone() const override;
};

class LastOfferJoinPolicy : public JoinPolicy
{
public:
    LastOfferJoinPolicy();                                       // constructor
    ~LastOfferJoinPolicy();                                      // destructor
    LastOfferJoinPolicy(const LastOfferJoinPolicy &);            // copy constructor
    LastOfferJoinPolicy(LastOfferJoinPolicy &&);                 // move contructor
    LastOfferJoinPolicy &operator=(const LastOfferJoinPolicy &); // copy assignment
    LastOfferJoinPolicy &operator=(LastOfferJoinPolicy &&);      // move assignment

    void join(Party &p, Simulation &s);
    virtual LastOfferJoinPolicy *clone() const override;
};