#include "Simulation.h"
#include <iostream>

using std::cout;
using std::endl;

Simulation::Simulation(Graph graph, vector<Agent> agents) : mGraph(graph), mAgents(agents)
{
}

void Simulation::initializeCoalitions()
{
    coalitions = {};
    for (int i = 0; i < mAgents.size(); i++)
    {
        Agent agent = mAgents.at(i);
        coalitions.push_back({agent.getPartyId()});
    }
}

void Simulation::addPartyToCoalition(int partyId, int coalitionId)
{
    for (vector<int> &coalition : coalitions)
    {
        for (int id : coalition)
        {
            if (id == coalitionId)
            {
                coalition.insert(coalition.end(), partyId);
                return;
            }
        }
    }

    cout << "no coalition found with party id of " << coalitionId << endl;
}

void Simulation::step()
{
    int numOfParties = mGraph.getNumVertices();
    for (int i = 0; i < numOfParties; i++)
    {
        Party party = mGraph.getParty(i);
        if (party.getState() != Joined)
            party.step(*this);
    }

    int numOfAgents = mAgents.size();
    for (int i = 0; i < numOfAgents; i++)
    {
        Agent agent = mAgents.at(i);
        agent.step(*this);
    }
}

bool Simulation::shouldTerminate() const
{
    bool shouldTerminate = true;

    int numOfParties = mGraph.getNumVertices();
    for (int i = 0; i < numOfParties; i++)
    {
        Party party = mGraph.getParty(i);
        State state = party.getState();
        if (state != Joined)
        {
            shouldTerminate = false;
        }
    }

    return shouldTerminate;
}

const Graph &Simulation::getGraph() const
{
    return mGraph;
}

const vector<Agent> &Simulation::getAgents() const
{
    return mAgents;
}

const Party &Simulation::getParty(int partyId) const
{
    return mGraph.getParty(partyId);
}

/// This method returns a "coalition" vector, where each element is a vector of party IDs in the coalition.
/// At the simulation initialization - the result will be [[agent0.partyId], [agent1.partyId], ...]
const vector<vector<int>> Simulation::getPartiesByCoalitions() const
{
    // TODO: you MUST implement this method for getting proper output, read the documentation above.
    return coalitions;
}
