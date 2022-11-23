#include <iostream>
#include "JoinPolicy.h"
#include "Simulation.h"

using std::cout;
using std::endl;
using std::vector;

LastOfferJoinPolicy::LastOfferJoinPolicy() : JoinPolicy() {}

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
    Coalition c = std::move(s.getCoalitionByPartyId(partyId));
    cout << "after getCoalitionByPartyId" << endl;
    c.addParty(p, s);
    cout << "Coalition c added Party p" << endl;
    s.cloneAgent(agent, lastOffered);
    cout << "cloneAgent done" << endl;
}

// void LastOfferJoinPolicy::addOffer(Agent &agent)
// {
//     cout << "inside last offer join policy add offer" << endl;
//     offers.push_back(agent);
// }