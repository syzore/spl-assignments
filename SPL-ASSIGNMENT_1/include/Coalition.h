#pragma once

#include <vector>
#include "Party.h"

using std::vector;

class Coalition
{
public:
    Coalition(Party &original);
    int getTotalMandates();
    void addParty(Party &p);
    void setMandates(Party &p);
    vector<int> getIdsVector() const;

private:
    int totalMandates;
    vector<Party> mParties;
};