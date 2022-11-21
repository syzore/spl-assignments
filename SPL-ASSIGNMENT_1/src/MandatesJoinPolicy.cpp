#include "JoinPolicy.h"
#include "Simulation.h"

void MandatesJoinPolicy::join(Party &p, Simulation &s)
{
    Coalition *favorite; // creats a dummy coalition to start with
    Agent *a;
    int mostMandates = -1;
    for (Agent &agent : offers)
    {
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

void MandatesJoinPolicy::addOffer(Agent &agent)
{
    offers.push_back(agent);
}