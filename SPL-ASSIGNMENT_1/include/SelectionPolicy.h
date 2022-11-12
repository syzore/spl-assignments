#pragma once

#include "JoinPolicy.h"

class SelectionPolicy
{
public:
    SelectionPolicy();
    void select();
};

class MandatesSelectionPolicy : public SelectionPolicy
{
public:
    void select();
};

class EdgeWeightSelectionPolicy : public SelectionPolicy
{
public:
    void select();
};