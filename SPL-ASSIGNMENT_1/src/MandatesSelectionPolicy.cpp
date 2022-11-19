#include "SelectionPolicy.h"
#include <algorithm>
using std::vector;

MandatesSelectionPolicy::MandatesSelectionPolicy() : SelectionPolicy(), alreadyOffered()
{
}

void MandatesSelectionPolicy::select(Agent &agent, Simulation &s)
{
    Graph g = s.getGraph();
    int total = g.getNumVertices();
    int maxMandates = -1;
    Party favorite = Party(-1, "dummy", 0, nullptr); // createing a dummy party
    for (int i = 0; i < total; i++)                  // finding favorite party by mandates parameter
    {
        Party party = g.getParty(i);
        if (party.getState() != Joined)
        {
            if (std::find(alreadyOffered.begin(), alreadyOffered.end(), i) == alreadyOffered.end())
            {
                int tempMandates = party.getMandates();
                if (tempMandates > maxMandates)
                {
                    maxMandates = tempMandates;
                    favorite = party;
                }
            }
        }
    }
    if (favorite.getId() != -1)
    {
        alreadyOffered.push_back(favorite.getId()); // add to alreadyOffered
        favorite.suggest(agent.getPartyId());       // party.suggest
    }
}