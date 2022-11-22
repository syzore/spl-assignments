#include <iostream>
#include "Coalition.h"
#include "Simulation.h"

using std::cout;
using std::endl;

Coalition::Coalition() : mParties(), totalMandates()
{
}

Coalition::Coalition(const Coalition &other) 
{
    cout << "other coalition total mandates: " << other.totalMandates << endl; 
    totalMandates = other.totalMandates;
    this->mParties = other.mParties;
}
int Coalition::getTotalMandates()
{
    return totalMandates;
}

void Coalition::addParty(Party &p, Simulation &s)
{
    mParties.push_back(p);
    setMandates(p);

    if (getTotalMandates() >= 61)
    {
        s.setTerminate(true);
    }
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
