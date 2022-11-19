#include "JoinPolicy.h"
#include "Simulation.h"

void LastOfferJoinPolicy::join(Party &p, Simulation &s)
{
    int idToJoin = offers.back();
    Coalition c = s.getCoalitionByPartyId(idToJoin);
    c.addParty(p);

    Coalition *cc;
}

void LastOfferJoinPolicy::addOffer(int partyId)
{
    offers.push_back(partyId);
}