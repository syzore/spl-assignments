#include "SelectionPolicy.h"
#include <algorithm>
#include <iostream>

using std::cout;
using std::endl;
using std::vector;

EdgeWeightSelectionPolicy::EdgeWeightSelectionPolicy() : SelectionPolicy()
{
}

void EdgeWeightSelectionPolicy::select(Agent &agent, Simulation &s)
{
    Graph g = s.getGraph();
    int total = g.getNumVertices();
    int maxWeight = 0;
    int selectedPartyId = -1;
    vector<int> alreadyOffered = agent.getMAlreadyOffered();
    for (int i = 0; i < total; i++)
    {
        Party party = g.getParty(i);
        if (party.getState() != Joined)
        {
            if (std::find(alreadyOffered.begin(), alreadyOffered.end(), i) == alreadyOffered.end())
            {
                int tempWeight = g.getEdgeWeight(i, agent.getPartyId());
                if (tempWeight > maxWeight)
                {
                    maxWeight = tempWeight;
                    selectedPartyId = party.getId();
                }
            }
        }
    }

    if (selectedPartyId != -1)
    {
        Party selected = g.getParty(selectedPartyId);
        agent.addToAlreadyOffered(selectedPartyId); // add to alreadyOffered
        selected.suggest(agent);
        s.updateAgent(agent);
        s.setParty(selected);
    }
    else
    {
        cout << "didnt find a new favorite in select" << endl;
    }
}