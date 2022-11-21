#include "SelectionPolicy.h"
#include <algorithm>
#include <iostream>

using std::cout;
using std::endl;
using std::vector;

EdgeWeightSelectionPolicy::EdgeWeightSelectionPolicy() : SelectionPolicy(), alreadyOffered()
{
}

void EdgeWeightSelectionPolicy::select(Agent &agent, Simulation &s)
{
    cout << "inside EdgeWeightSelectionPolicy" << endl;

    Graph g = s.getGraph();
    int total = g.getNumVertices();
    int maxWeight = -1;
    Party *favorite;
    for (int i = 0; i < total; i++) // finding favorite party by edge weight parameter
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
                    favorite = &party;
                }
            }
        }
    }

    cout << "after for loop" << endl;

    if (favorite == nullptr)
        return;

    int partyId = favorite->getId();
    if (partyId != -1)
    {
        cout << "inside if" << endl;

        alreadyOffered.push_back(partyId); // add to alreadyOffered

        cout << "after vector push" << endl;

        favorite->suggest(agent);
    }
    else
    {
        cout << "didnt find a new favorite in select" << endl;
    }

    favorite = nullptr;
    delete favorite;
}