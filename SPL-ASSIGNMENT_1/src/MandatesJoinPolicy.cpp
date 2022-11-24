#include <iostream>

#include "JoinPolicy.h"
#include "Simulation.h"

using std::cout;
using std::endl;

MandatesJoinPolicy::MandatesJoinPolicy() : JoinPolicy() {}

void MandatesJoinPolicy::join(Party &p, Simulation &s)
{
    cout << "inside mandats join policy join" << endl;
    int coalitionId = -1;
    int agentId = -1;
    int mostMandates = -1;
    // vector<Agent> offers = p.getMOffers();
    vector<int> offers = p.getMOffers();
    for (int i : offers)
    {
        Agent agent = s.getAgentByPartyId(i);
        Coalition temp = s.getCoalitionByPartyId(agent.getPartyId());
        int coalitionMandates = temp.getTotalMandates();
        if (coalitionMandates > mostMandates)
        {
            mostMandates = coalitionMandates;
            coalitionId = temp.getCoalitionId();
            agentId = agent.getId();
        }
    }

    if (agentId != -1){
        Agent a = s.getAgents().at(agentId);
        s.cloneAgent(a, p.getId());
    } 
    Coalition c = s.getCoalitionById(coalitionId);
    c.addParty(p, s);
    s.setCoalition(c);
}

MandatesJoinPolicy* MandatesJoinPolicy::clone() const
{
 return new MandatesJoinPolicy;
}


