#include "JoinPolicy.h"
#include "Simulation.h"

void MandatesJoinPolicy::join(Party &p, Simulation &s)
{
    Coalition* favorite = nullptr;  //creats a dummy coalition to start with
    int mostMandates = -1;
    for (int partyId : offers)
    {
        Coalition temp = s.getCoalitionByPartyId(partyId);
        int coalitionMandates = temp.getTotalMandates();
        if (coalitionMandates > mostMandates)
        {
            mostMandates = coalitionMandates;
            favorite = &temp;
        }
    }
    favorite->addParty(p);
}

void MandatesJoinPolicy::addOffer(int partyId)
{
    offers.push_back(partyId);
}