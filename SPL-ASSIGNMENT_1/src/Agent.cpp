#include <iostream>
#include "Agent.h"
#include "Simulation.h"
#include "SelectionPolicy.h"

using std::cout;
using std::endl;
using std::move;
using std::vector;

Agent::Agent(int agentId, int partyId, SelectionPolicy *selectionPolicy) : mAgentId(agentId), mPartyId(partyId), mSelectionPolicy(selectionPolicy), mAlreadyOffered()
{
    // You can change the implementation of the constructor, but not the signature!
}

// copy constructor
Agent::Agent(const Agent &other) : mAgentId(other.mAgentId), mPartyId(other.mPartyId), mSelectionPolicy(), mAlreadyOffered(other.mAlreadyOffered)
{
    mSelectionPolicy = other.mSelectionPolicy->clone();
}

Agent::Agent(Agent &&other) : mAgentId(other.mAgentId), mPartyId(other.mPartyId), mSelectionPolicy(), mAlreadyOffered(other.mAlreadyOffered)
{
    mSelectionPolicy = other.mSelectionPolicy;
    other.mSelectionPolicy = nullptr;
}

Agent &Agent::operator=(const Agent &other)
{
    // cout << "inside agent copy assignment" << endl;
    if (this != &other)
    {
        delete mSelectionPolicy;
        mSelectionPolicy = other.mSelectionPolicy->clone();
        mAgentId = other.mAgentId;
        mPartyId = other.mPartyId;
        mAlreadyOffered = other.mAlreadyOffered;
    }
    return *this;
}

Agent &Agent::operator=(Agent &&other)
{
    // cout << "inside agent move assignment" << endl;
    mSelectionPolicy = other.mSelectionPolicy;
    mAgentId = move(other.mAgentId);
    mPartyId = move(other.mPartyId);
    mAlreadyOffered = other.mAlreadyOffered;

    other.mSelectionPolicy = nullptr;
    delete other.mSelectionPolicy;

    return *this;
}

Agent::~Agent()
{
    if (mSelectionPolicy)
    {
        delete mSelectionPolicy;
        mSelectionPolicy = nullptr;
    }
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
    (*mSelectionPolicy).select(*this, sim);
}

vector<int> Agent::getMAlreadyOffered() const
{
    return mAlreadyOffered;
}

void Agent::addToAlreadyOffered(int id)
{
    cout << "added offer, party id = " << id << ", to agent id = " << mAgentId << endl;
    mAlreadyOffered.push_back(id);
}