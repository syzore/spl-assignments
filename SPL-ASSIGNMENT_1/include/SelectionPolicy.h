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
    MandatesSelectionPolicy(const MandatesSelectionPolicy &);            // copy constructor
    MandatesSelectionPolicy(MandatesSelectionPolicy &&);                 // move contructor
    MandatesSelectionPolicy &operator=(const MandatesSelectionPolicy &); // copy assignment
    MandatesSelectionPolicy &operator=(MandatesSelectionPolicy &&);      // move assignment

    virtual void select(Agent &agent, Simulation &s) override;
    virtual MandatesSelectionPolicy *clone() const override;
};

class EdgeWeightSelectionPolicy : public SelectionPolicy
{
public:
    EdgeWeightSelectionPolicy();
    ~EdgeWeightSelectionPolicy();
    EdgeWeightSelectionPolicy(const EdgeWeightSelectionPolicy &);            // copy constructor
    EdgeWeightSelectionPolicy(EdgeWeightSelectionPolicy &&);                 // move contructor
    EdgeWeightSelectionPolicy &operator=(const EdgeWeightSelectionPolicy &); // copy assignment
    EdgeWeightSelectionPolicy &operator=(EdgeWeightSelectionPolicy &&);      // move assignment

    virtual void select(Agent &agent, Simulation &s) override;
    virtual EdgeWeightSelectionPolicy *clone() const override;
};