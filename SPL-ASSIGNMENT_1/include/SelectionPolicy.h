#pragma once

#include "JoinPolicy.h"
#include "Agent.h"
#include "Simulation.h"


class SelectionPolicy
{
public:
    SelectionPolicy(){};
    virtual void select() = 0;
};

class MandatesSelectionPolicy : public SelectionPolicy
{
public:
    MandatesSelectionPolicy() = default;
    void select();
private:
    vector<int> alreadyOffered;
};

class EdgeWeightSelectionPolicy : public SelectionPolicy
{
public:
    EdgeWeightSelectionPolicy() = default;
    void select(Agent &agent, Simulation &s);
private:
    vector<int> alreadyOffered;
};