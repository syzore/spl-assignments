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
    cout << "inside EdgeWeightSelectionPolicy" << endl;
    Graph g = s.getGraph();
    int total = g.getNumVertices();
    int maxWeight = -1;
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
    cout << "after for loop" << endl;
    if (selectedPartyId != -1)
    {
        Party selected = g.getParty(selectedPartyId);
        cout << "inside if" << endl;
        alreadyOffered.push_back(selectedPartyId); // add to alreadyOffered
        cout << "after vector push" << endl;
        selected.suggest(agent);
        cout << "printing number of offerrs in edge weight SP" << endl;
        selected.printNumberOfOffers();
        s.updateAgent(agent);
        s.setParty(selected);
        Party testP = s.getParty(selected.getId());
        cout << "printing number of offers TEST in edge weight SP" << endl;
        testP.printNumberOfOffers();
    }
    else
    {
        cout << "didnt find a new favorite in select" << endl;
    }
}

// void EdgeWeightSelectionPolicy::select(Agent &agent, Simulation &s)
// {
//     cout << "inside EdgeWeightSelectionPolicy" << endl;

//     Graph g = s.getGraph();
//     int total = g.getNumVertices();
//     int maxWeight = -1;
//     int selectedPartyId = -1;
//     for (int i = 0; i < total; i++) // finding favorite party by edge weight parameter
//     {
//         Party party = g.getParty(i);
//         if (party.getState() != Joined)
//         {
//             if (std::find(alreadyOffered.begin(), alreadyOffered.end(), i) == alreadyOffered.end())
//             {
//                 int tempWeight = g.getEdgeWeight(i, agent.getPartyId());
//                 if (tempWeight > maxWeight)
//                 {
//                     maxWeight = tempWeight;
//                     selectedPartyId = party.getId();
//                 }
//             }
//         }
//     }

//     cout << "after for loop" << endl;

//     if (selectedPartyId != -1)
//     {
//         Party selected = g.getParty(selectedPartyId);
//         cout << "inside if" << endl;

//         alreadyOffered.push_back(selectedPartyId); // add to alreadyOffered

//         cout << "after vector push" << endl;

//         selected.suggest(agent);

//         cout << "printing number of offerrs in edge weight SP" << endl;
//         selected.printNumberOfOffers();

//         s.updateAgent(agent);
//         s.setParty(selected);

//         Party testP = s.getParty(selected.getId());
//         cout << "printing number of offers TEST in edge weight SP" << endl;
//         testP.printNumberOfOffers();
//     }
//     else
//     {
//         cout << "didnt find a new favorite in select" << endl;
//     }
// }