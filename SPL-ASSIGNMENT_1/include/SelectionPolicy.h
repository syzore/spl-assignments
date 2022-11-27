#pragma once

#include "Simulation.h"

class Agent;
class SelectionPolicy
{
public:
    SelectionPolicy() = default;
    virtual ~SelectionPolicy(){};
    virtual void select(Agent &agent, Simulation &s) = 0;
    virtual SelectionPolicy *clone() const = 0;
};

class MandatesSelectionPolicy : public SelectionPolicy
{
public:
    MandatesSelectionPolicy();
    ~MandatesSelectionPolicy();
    virtual void select(Agent &agent, Simulation &s) override;
    virtual MandatesSelectionPolicy *clone() const override;
};

class EdgeWeightSelectionPolicy : public SelectionPolicy
{
public:
    EdgeWeightSelectionPolicy();
    ~EdgeWeightSelectionPolicy();
    virtual void select(Agent &agent, Simulation &s) override;
    virtual EdgeWeightSelectionPolicy *clone() const override;
};