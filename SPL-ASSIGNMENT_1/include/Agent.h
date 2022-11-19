#pragma once

#include <vector>
#include "Graph.h"

class SelectionPolicy;

class Agent
{
public:
    Agent(int agentId, int partyId, SelectionPolicy *selectionPolicy);
    Agent(Agent& clone);

    int getPartyId() const;
    void setPartyId(int partyId);
    int getId() const;
    void setId(int Id);
    void step(Simulation &);
    

private:
    int mAgentId;
    int mPartyId;
    SelectionPolicy *mSelectionPolicy;
};
