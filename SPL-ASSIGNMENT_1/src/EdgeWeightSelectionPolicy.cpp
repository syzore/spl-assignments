#include "SelectionPolicy.h"
#include <algorithm>
using std::vector;

EdgeWeightSelectionPolicy::EdgeWeightSelectionPolicy() : SelectionPolicy(), alreadyOffered()
{
}

void EdgeWeightSelectionPolicy::select(Agent &agent, Simulation &s)
{
    Graph g = s.getGraph();
    int total = g.getNumVertices();
    int maxWeight = -1;
    Party favorite = Party(-1, "dummy", 0, nullptr); // createing a dummy party
    for (int i = 0; i < total; i++)                  // finding favorite party by edge weight parameter
    {
        if (g.getParty(i).getState() != Joined)
        {
            if (std::find(alreadyOffered.begin(), alreadyOffered.end(), i) == alreadyOffered.end())
            {
                int tempWeight = g.getEdgeWeight(i, agent.getPartyId());
                if (tempWeight > maxWeight)
                {
                    maxWeight = tempWeight;
                    favorite = g.getParty(i);
                }
            }
        }
    }
    if (favorite.getId() != -1)
    {
        alreadyOffered.push_back(favorite.getId()); // add to alreadyOffered
        favorite.suggest(agent);
    }
}