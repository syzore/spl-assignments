#include "JoinPolicy.h"
#include "Simulation.h"

void LastOfferJoinPolicy::join(Party &p, Simulation &s)
{
    Agent &lastOffered = offers.back();
    int idToJoin = lastOffered.getPartyId();
    Coalition c = s.getCoalitionByPartyId(idToJoin);
    c.addParty(p, s);

    s.cloneAgent(lastOffered, p.getId());
}

void LastOfferJoinPolicy::addOffer(Agent &agent)
{
    offers.push_back(agent);
}