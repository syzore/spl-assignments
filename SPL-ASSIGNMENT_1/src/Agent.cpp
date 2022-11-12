#include "Agent.h"
#include "SelectionPolicy.h"
#include "JoinPolicy.h"
#include "Graph.h"
#include "Simulation.h"

Agent::Agent(int agentId, int partyId, SelectionPolicy *selectionPolicy) : mAgentId(agentId), mPartyId(partyId), mSelectionPolicy(selectionPolicy)
{
    // You can change the implementation of the constructor, but not the signature!
}

int Agent::getId() const
{
    return mAgentId;
}

int Agent::getPartyId() const
{
    return mPartyId;
}

void Agent::step(Simulation &sim)
{
    Graph graph = sim.getGraph();
    int numOfParties = graph.getNumVertices();
    for (int i = 0; i < numOfParties; i++)
    {
        Party party = graph.getParty(i);
        State state = party.getState();
        if (state != Joined)
        {
        }
    }

    (*mSelectionPolicy).select();

    // TODO: implement this method
}
