#include "SelectionPolicy.h"
#include <algorithm>
#include <iostream>

using std::cout;
using std::endl;
using std::vector;

MandatesSelectionPolicy::MandatesSelectionPolicy() : SelectionPolicy() {}

MandatesSelectionPolicy::~MandatesSelectionPolicy() {}

void MandatesSelectionPolicy::select(Agent &agent, Simulation &s)
{
    cout << "inside mandates selection policy" << endl;

    Graph g = s.getGraph();
    int total = g.getNumVertices();
    int maxMandates = -1;
    int selectedPartyId = -1;
    vector<int> alreadyOffered = agent.getMAlreadyOffered();
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
                    selectedPartyId = party.getId();
                }
            }
        }
    }

    if (selectedPartyId != -1)
    {
        Party selected = g.getParty(selectedPartyId);
        agent.addToAlreadyOffered(selectedPartyId); // add to alreadyOffered
        selected.suggest(agent);                    // party.suggest

        s.updateAgent(agent);
        s.setParty(selected);
    }
    else
    {
        cout << "didnt find a new favorite in MandatesSelectionPolicy select" << endl;
    }
}

MandatesSelectionPolicy *MandatesSelectionPolicy::clone() const
{
    return new MandatesSelectionPolicy;
}