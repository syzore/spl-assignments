#include <iostream>
#include "Agent.h"
#include "Simulation.h"
#include "SelectionPolicy.h"

using std::cout;
using std::endl;

Agent::Agent(int agentId, int partyId, SelectionPolicy *selectionPolicy) : mAgentId(agentId), mPartyId(partyId), mSelectionPolicy(selectionPolicy)
{
    // You can change the implementation of the constructor, but not the signature!
}

Agent::Agent(const Agent &clone)
{
    mSelectionPolicy = clone.mSelectionPolicy;
    mAgentId = clone.getId();
    mPartyId = clone.getPartyId();
}

Agent::~Agent()
{
    mSelectionPolicy = nullptr;
    delete mSelectionPolicy;
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
