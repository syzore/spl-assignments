#include "SelectionPolicy.h"
#include <algorithm>
#include <iostream>

using std::cout;
using std::endl;
using std::vector;

MandatesSelectionPolicy::MandatesSelectionPolicy() : SelectionPolicy()
{
}

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
        alreadyOffered.push_back(selectedPartyId); // add to alreadyOffered
        cout << "Selected party state = " << selected.getState() << endl;
        selected.suggest(agent); // party.suggest

        cout << "printing number of offerrs in mandates SP" << endl;
        selected.printNumberOfOffers();

        s.updateAgent(agent);
        s.setParty(selected);

        Party testP = s.getParty(selected.getId());
        cout << "printing number of offers TEST in mandates SP" << endl;
        testP.printNumberOfOffers();
    }
    else
    {
        cout << "didnt find a new favorite in MandatesSelectionPolicy select" << endl;
    }
}