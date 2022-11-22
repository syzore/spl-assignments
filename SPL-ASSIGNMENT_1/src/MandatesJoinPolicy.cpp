#include <iostream>

#include "JoinPolicy.h"
#include "Simulation.h"

using std::cout;
using std::endl;

MandatesJoinPolicy::MandatesJoinPolicy() : offers() {}

void MandatesJoinPolicy::join(Party &p, Simulation &s)
{
    cout << "inside mandats join policy join" << endl;
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
    cout << "inside mandates join policy add offer" << endl;
    offers.push_back(agent);
}

int MandatesJoinPolicy::getOffersSize() const
{
    return offers.size();
}