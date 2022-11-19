#include "JoinPolicy.h"
#include "Simulation.h"

void LastOfferJoinPolicy::join(Party &p, Simulation &s)
{
    int idToJoin = offers.back();
    
}

void LastOfferJoinPolicy::addOffer(int partyId)
{
    offers.push_back(partyId);
}