#pragma once

#include "Graph.h"

class SelectionPolicy;

class Agent
{
public:
    Agent(int agentId, int partyId, SelectionPolicy *selectionPolicy);
    Agent (const Agent&) = delete; //copy constructor
    Agent (Agent&&) = default; // move constructor
    Agent& operator=(const Agent&) = default; //copy assignment
    Agent& operator=(Agent&&) = delete; //move assignment
    Agent(const Agent &clone);
    ~Agent();

    int getPartyId() const;
    void setPartyId(int partyId);
    int getId() const;
    void setId(int Id);
    void step(Simulation &sim);

private:
    int mAgentId;
    int mPartyId;
    SelectionPolicy *mSelectionPolicy;
};
