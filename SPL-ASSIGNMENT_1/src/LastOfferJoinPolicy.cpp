#include <iostream>
#include "JoinPolicy.h"
#include "Simulation.h"

using std::cout;
using std::endl;

LastOfferJoinPolicy::LastOfferJoinPolicy() : offers() {}

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
    cout << "inside last offer join policy add offer" << endl;
    offers.push_back(agent);
}