#pragma once

#include <vector>
// #include "Simulation.h"

using std::vector;

class Simulation;
class Party;

class Coalition
{
public:
    Coalition();
    Coalition(const Coalition &);                                   // copy constructor
    int getTotalMandates();
    void addParty(Party &p, Simulation &s);
    void setMandates(Party &p);
    vector<int> getIdsVector() const;

private:
    int totalMandates;
    vector<Party> mParties;
};