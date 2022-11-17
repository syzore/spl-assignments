#include "Coalition.h"
#include "Party.h"

Coalition::Coalition(Party &original) : mParties()
{
    mParties = {original};
    totalMandates = original.getMandates();
}

int Coalition::getTotalMandates()
{
    return totalMandates;
}

void Coalition::addParty(Party &p)
{
    mParties.push_back(p);
    setMandates(p);
}

void Coalition::setMandates(Party &p)
{
    totalMandates += p.getMandates();
}

vector<int> Coalition::getIdsVector() const
{
    vector<int> output = {};
    for (Party p : mParties)
    {
        output.push_back(p.getId());
    }
    return output;
}
