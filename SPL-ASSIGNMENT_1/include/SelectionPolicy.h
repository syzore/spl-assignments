#pragma once

#include "Simulation.h"

class Agent;
class SelectionPolicy
{
public:
    SelectionPolicy(){};
    virtual void select(Agent &agent, Simulation &s) = 0;
};

class MandatesSelectionPolicy : public SelectionPolicy
{
public:
    MandatesSelectionPolicy();
    void select(Agent &agent, Simulation &s) override;

private:
    vector<int> alreadyOffered;
};

class EdgeWeightSelectionPolicy : public SelectionPolicy
{
public:
    EdgeWeightSelectionPolicy();
    void select(Agent &agent, Simulation &s) override;

private:
    vector<int> alreadyOffered; // saves the parties a given agent alreay offered
};