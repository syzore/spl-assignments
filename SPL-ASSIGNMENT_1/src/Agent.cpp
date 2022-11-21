#include <iostream>
#include "Agent.h"
#include "Simulation.h"
#include "SelectionPolicy.h"

using std::cout;
using std::endl;
using std::move;

Agent::Agent(int agentId, int partyId, SelectionPolicy *selectionPolicy) : mAgentId(agentId), mPartyId(partyId), mSelectionPolicy(selectionPolicy)
{
    // You can change the implementation of the constructor, but not the signature!
}

Agent::Agent(const Agent &other)
{
    this->mAgentId = other.mAgentId;
    this->mPartyId = other.mPartyId;

    cout << "mSelectionPolicy = " << mSelectionPolicy << endl;

    if (typeid(*mSelectionPolicy) == typeid(MandatesSelectionPolicy))
    {
        this->mSelectionPolicy = new MandatesSelectionPolicy;
    }
    else
    {
        this->mSelectionPolicy = new EdgeWeightSelectionPolicy;
    }
}

Agent::Agent(Agent &&other)
{
    this->mAgentId = other.mAgentId;
    this->mPartyId = other.mPartyId;
    if (typeid(*mSelectionPolicy) == typeid(MandatesSelectionPolicy))
    {
        this->mSelectionPolicy = new MandatesSelectionPolicy;
    }
    else
    {
        this->mSelectionPolicy = new EdgeWeightSelectionPolicy;
    }
    other.mSelectionPolicy = nullptr;
}

Agent &Agent::operator=(const Agent &other)
{
    if (this != &other)
    {
        *mSelectionPolicy = *(other.mSelectionPolicy);
        this->mAgentId = other.mAgentId;
        this->mPartyId = other.mPartyId;
    }
    return *this;
}

Agent &Agent::operator=(Agent &&other)
{
    *mSelectionPolicy = *(other.mSelectionPolicy);
    this->mAgentId = move(other.mAgentId);
    this->mPartyId = move(other.mPartyId);
    return *this;
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
    // Graph graph = sim.getGraph();
    // int numOfParties = graph.getNumVertices();
    // for (int i = 0; i < numOfParties; i++)
    // {
    //     Party party = graph.getParty(i);
    //     State state = party.getState();
    //     if (state != Joined)
    //     {

    //     }
    // }

    (*mSelectionPolicy).select(*this, sim);
}
