#include "JoinPolicy.h"
#include "Simulation.h"

void MandatesJoinPolicy::join(Party &p, Simulation &s)
{
    for (int partyId : offers)
    {
    }
}

void MandatesJoinPolicy::addOffer(int partyId)
{
    offers.push_back(partyId);
}