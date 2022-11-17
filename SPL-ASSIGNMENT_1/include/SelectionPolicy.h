#pragma once

#include "JoinPolicy.h"
#include "Agent.h"
#include "Simulation.h"


class SelectionPolicy
{
public:
    SelectionPolicy();
    void select();

    
};

class MandatesSelectionPolicy : public SelectionPolicy
{
public:
    MandatesSelectionPolicy();
    void select();
private:
    vector<int> alreadyOffered;
};

class EdgeWeightSelectionPolicy : public SelectionPolicy
{
public:
    EdgeWeightSelectionPolicy();
    void select(Agent &agent, Simulation &s);
private:
    vector<int> alreadyOffered;
};