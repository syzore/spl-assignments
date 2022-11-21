#pragma once

#include "Simulation.h"

class Agent;
class SelectionPolicy
{
public:
    SelectionPolicy() = default;
    virtual void select(Agent &agent, Simulation &s) = 0;
};

class MandatesSelectionPolicy : public SelectionPolicy
{
public:
    MandatesSelectionPolicy();
    virtual void select(Agent &agent, Simulation &s) override;

private:
    vector<int> alreadyOffered;
};

class EdgeWeightSelectionPolicy : public SelectionPolicy
{
public:
    EdgeWeightSelectionPolicy();
    virtual void select(Agent &agent, Simulation &s) override;

private:
    vector<int> alreadyOffered; // saves the parties a given agent alreay offered
};