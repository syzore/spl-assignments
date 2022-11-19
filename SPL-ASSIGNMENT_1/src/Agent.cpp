#include "Agent.h"
#include "SelectionPolicy.h"
#include "JoinPolicy.h"
#include "Graph.h"
#include "Simulation.h"

Agent::Agent(int agentId, int partyId, SelectionPolicy *selectionPolicy) : mAgentId(agentId), mPartyId(partyId), mSelectionPolicy(selectionPolicy)
{
    // You can change the implementation of the constructor, but not the signature!
}

Agent::Agent(Agent& clone)
{
    mSelectionPolicy = clone.mSelectionPolicy;
    mAgentId = -1;
    mPartyId = -1;
}

int Agent::getId() const
{
    return mAgentId;
}

void Agent::setId(int Id)
{
    mAgentId = Id;
}

int Agent::getPartyId() const
{
    return mPartyId;
}

void Agent::setPartyId(int partyId)
{
    mPartyId = partyId;
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

    (*mSelectionPolicy).select(*this, sim);

    // TODO: implement this method
}
