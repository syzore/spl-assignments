#pragma once

#include "JoinPolicy.h"

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
};

class EdgeWeightSelectionPolicy : public SelectionPolicy
{
public:
    EdgeWeightSelectionPolicy() = default;
    void select();
};