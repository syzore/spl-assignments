#pragma once

#include <vector>
// #include "Simulation.h"

using std::vector;

class Simulation;
class Party;

class Coalition
{
public:
    Coalition();                             // constructor
    ~Coalition();                            // destructor
    Coalition(const Coalition &);            // copy constructor
    Coalition(Coalition &&);                 // move contructor
    Coalition &operator=(const Coalition &); // copy assignment
    Coalition &operator=(Coalition &&);      // move assignment

    int getTotalMandates() const;
    void addParty(Party &p, Simulation &s);
    void setMandates(Party &p);
    vector<int> getIdsVector() const;

private:
    int totalMandates;
    vector<Party> mParties;
};