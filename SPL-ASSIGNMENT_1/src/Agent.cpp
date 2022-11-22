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
Agent::Agent(const Agent &other)
{
    this->mAgentId = other.mAgentId;
    this->mPartyId = other.mPartyId;

    if (typeid(other.mSelectionPolicy) == typeid(MandatesSelectionPolicy))
    {
        mSelectionPolicy = new MandatesSelectionPolicy;
    }
    else
    {
        mSelectionPolicy = new EdgeWeightSelectionPolicy;
    }
    this->mAlreadyOffered = other.mAlreadyOffered;
}

Agent::Agent(Agent &&other)
{
    cout << "inside agent move constructor" << endl;
    this->mAgentId = other.mAgentId;
    this->mPartyId = other.mPartyId;
    if (typeid(other.mSelectionPolicy) == typeid(MandatesSelectionPolicy))
    {
        this->mSelectionPolicy = new MandatesSelectionPolicy;
    }
    else
    {
        this->mSelectionPolicy = new EdgeWeightSelectionPolicy;
    }
    other.mSelectionPolicy = nullptr;
    this->mAlreadyOffered = other.mAlreadyOffered;  
    cout << "finish agent move constructor" << endl;
}

Agent &Agent::operator=(const Agent &other)
{
    cout << "inside agent copy assignment" << endl;
    if (this != &other)
    {
        *mSelectionPolicy = *(other.mSelectionPolicy);
        this->mAgentId = other.mAgentId;
        this->mPartyId = other.mPartyId;
    }
    this->mAlreadyOffered = other.mAlreadyOffered;
    return *this;
}

Agent &Agent::operator=(Agent &&other)
{
    cout << "inside agent move assignment" << endl;
    *mSelectionPolicy = *(other.mSelectionPolicy);
    this->mAgentId = move(other.mAgentId);
    this->mPartyId = move(other.mPartyId);
    this->mAlreadyOffered = other.mAlreadyOffered;
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
    (*mSelectionPolicy).select(*this, sim);
}

vector<int> Agent::getMAlreadyOffered() const
{
    return mAlreadyOffered;
}