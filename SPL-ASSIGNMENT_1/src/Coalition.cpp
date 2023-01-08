#include <iostream>
#include "Coalition.h"
#include "Simulation.h"

using std::cout;
using std::endl;

// constructor
Coalition::Coalition(int id) : id(id), totalMandates(), mParties()
{
}

// destructor
Coalition::~Coalition()
{
}

// copy constructor
Coalition::Coalition(const Coalition &other) : id(other.id), totalMandates(other.getTotalMandates()), mParties(other.mParties)
{
}

// move constructor
Coalition::Coalition(Coalition &&other) : id(), totalMandates(), mParties()
{
    // cout << "move constructor was called inside Coalition" << endl;
    totalMandates = std::move(other.getTotalMandates());
    mParties = std::move(other.mParties);
    id = std::move(other.id);
}

// copy assignment
Coalition &Coalition::operator=(const Coalition &rhs)
{
    // cout << "copy assignment was called inside Coalition" << endl;
    totalMandates = rhs.getTotalMandates();
    mParties = rhs.mParties;
    id = rhs.id;
    return *this;
}

// move assignment
Coalition &Coalition::operator=(Coalition &&rhs)
{
    // cout << "move assignment was called inside Coalition" << endl;
    totalMandates = std::move(rhs.totalMandates);
    mParties = std::move(rhs.mParties);
    id = std::move(rhs.id);
    return *this;
}

int Coalition::getTotalMandates() const
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

const int Coalition::getCoalitionId() const
{
    return id;
}
