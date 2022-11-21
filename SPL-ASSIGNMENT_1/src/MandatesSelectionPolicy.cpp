#include "SelectionPolicy.h"
#include <algorithm>
#include <iostream>

using std::cout;
using std::endl;
using std::vector;

MandatesSelectionPolicy::MandatesSelectionPolicy() : SelectionPolicy(), alreadyOffered()
{
}

void MandatesSelectionPolicy::select(Agent &agent, Simulation &s)
{
    cout << "inside mandates selection policy" << endl;

    Graph g = s.getGraph();
    int total = g.getNumVertices();
    int maxMandates = -1;
    Party *favorite;
    for (int i = 0; i < total; i++) // finding favorite party by mandates parameter
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
                    favorite = &party;
                }
            }
        }
    }

    if (favorite == nullptr)
    {
        cout << "favorite in MandatesSelectionPolicy is null" << endl;
        return;
    }
    if (favorite->getId() != -1)
    {
        alreadyOffered.push_back(favorite->getId()); // add to alreadyOffered
        favorite->suggest(agent);                    // party.suggest
    }
    else
    {
        cout << "didnt find a new favorite in MandatesSelectionPolicy select" << endl;
    }

    favorite = nullptr;
    delete favorite;
}