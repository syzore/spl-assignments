#pragma once

#include <vector>

#include "Graph.h"
#include "Agent.h"
#include "Coalition.h"

using std::string;
using std::vector;

class Simulation
{
public:
    Simulation(Graph g, vector<Agent> agents);

    void step();
    bool shouldTerminate() const;

    const Graph &getGraph() const;

    const vector<Agent> &getAgents() const;
    const Party &getParty(int partyId) const;
    void setParty(Party &);
    const vector<vector<int>> getPartiesByCoalitions() const;
    void cloneAgent(Agent &a, int partyId);
    void reduceAvailableParties();
    void setTerminate(bool t);
    Coalition &getCoalitionByPartyId(int id) const;
    void initializeCoalitions();

private:
    Graph mGraph;
    vector<Agent> mAgents;
    vector<Coalition> mCoalitions;
    int availableParties;
    bool terminate;
};
