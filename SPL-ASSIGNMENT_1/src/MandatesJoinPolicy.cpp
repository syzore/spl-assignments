#include <iostream>

#include "JoinPolicy.h"
#include "Simulation.h"

using std::cout;
using std::endl;

MandatesJoinPolicy::MandatesJoinPolicy() : JoinPolicy() {}

void MandatesJoinPolicy::join(Party &p, Simulation &s)
{
    cout << "inside mandats join policy join" << endl;
    Coalition *favorite; // creats a dummy coalition to start with
    Agent *a;
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
            Coalition *pointer = &temp;
            favorite = pointer;
            a = &agent;
        }
    }

    if (a != nullptr)
        s.cloneAgent(*a, p.getId());

    favorite->addParty(p, s);
}

MandatesJoinPolicy* MandatesJoinPolicy::clone() const
{
 return new MandatesJoinPolicy;
}


