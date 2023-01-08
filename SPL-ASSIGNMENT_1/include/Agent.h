#pragma once

#include "Graph.h"

class SelectionPolicy;

class Agent
{
public:
    Agent(int agentId, int partyId, SelectionPolicy *selectionPolicy);
    Agent(const Agent &);            // copy constructor
    Agent(Agent &&);                 // move constructor
    Agent &operator=(const Agent &); // copy assignment
    Agent &operator=(Agent &&);      // move assignment
    ~Agent();                        // destructor

    int getPartyId() const;
    void setPartyId(int partyId);
    int getId() const;
    void setId(int Id);
    void step(Simulation &sim);
    vector<int> getMAlreadyOffered() const;
    void addToAlreadyOffered(int);

private:
    int mAgentId;
    int mPartyId;
    SelectionPolicy *mSelectionPolicy;
    vector<int> mAlreadyOffered; // saves the parties a given agent alreay offered
};
