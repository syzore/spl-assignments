#include <iostream>
#include "JoinPolicy.h"
#include "Simulation.h"

using std::cout;
using std::endl;
using std::vector;

LastOfferJoinPolicy::LastOfferJoinPolicy() : JoinPolicy() {}

LastOfferJoinPolicy::~LastOfferJoinPolicy() {}

void LastOfferJoinPolicy::join(Party &p, Simulation &s)
{
    cout << "inside last offer join" << endl;
    vector<int> offers = p.getMOffers();
    // Agent &lastOffered = offers.back();
    // cout << "found offer.back" << endl;
    cout << "offers.size = " << offers.size() << endl;
    int lastOffered = offers.back();
    Agent agent = s.getAgentByPartyId(lastOffered);
    int partyId = agent.getPartyId();
    Coalition c = s.getCoalitionByPartyId(partyId);
    c.addParty(p, s);
    s.setCoalition(c);
    s.cloneAgent(agent, lastOffered);
}

LastOfferJoinPolicy *LastOfferJoinPolicy::clone() const
{
    return new LastOfferJoinPolicy;
}
