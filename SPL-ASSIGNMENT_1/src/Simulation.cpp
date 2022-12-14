#include "Simulation.h"
#include <iostream>
#include <algorithm>

using std::cout;
using std::endl;

Simulation::Simulation(Graph graph, vector<Agent> agents) : mGraph(graph), mAgents(agents), mCoalitions(), availableParties(graph.getNumVertices()), terminate(false)
{
}

void Simulation::initializeCoalitions()
{
    for (long unsigned int i = 0; i < mAgents.size(); i++)
    {
        Agent agent = mAgents.at(i);
        Party originalParty = mGraph.getParty(agent.getPartyId());
        Coalition coalition = Coalition(i);
        coalition.addParty(originalParty, *this);
        mCoalitions.push_back(coalition);
        reduceAvailableParties();
    }
}

void Simulation::step()
{
    int numOfParties = mGraph.getNumVertices();
    for (int i = 0; i < numOfParties; i++)
    {
        Party party = mGraph.getParty(i);
        if (party.getState() != Joined)
            party.step(*this);
    }

    int numOfAgents = mAgents.size();
    for (int i = 0; i < numOfAgents; i++)
    {
        Agent agent = mAgents.at(i);
        agent.step(*this);
    }
}

bool Simulation::shouldTerminate() const
{
    return terminate || availableParties == 0;
}

void Simulation::setTerminate(bool t)
{
    terminate = t;
}

void Simulation::reduceAvailableParties()
{
    availableParties--;
}

const Graph &Simulation::getGraph() const
{
    return mGraph;
}

void Simulation::setGraph(Graph &graph)
{
    mGraph = graph;
}

const vector<Agent> &Simulation::getAgents() const
{
    return mAgents;
}

Agent Simulation::getAgentByPartyId(int partyId)
{
    long unsigned int index = 0;
    while (index < mAgents.size())
    {
        Agent agent = mAgents.at(index);

        if (agent.getPartyId() == partyId)
            break;

        index++;
    }

    return mAgents.at(index);
}

void Simulation::setAgents(vector<Agent> &agents)
{
    mAgents = agents;
}

void Simulation::updateAgent(Agent &agent)
{
    vector<Agent> agents = getAgents();
    agents.at(agent.getId()) = agent;
    setAgents(agents);
}

void Simulation::cloneAgent(Agent &a, int partyId)
{
    Agent clone = Agent(a);
    clone.setPartyId(partyId);
    clone.setId(getAgents().size());

    mAgents.push_back(clone);
}

const Party &Simulation::getParty(int partyId) const
{
    return mGraph.getParty(partyId);
}

void Simulation::setParty(Party &party)
{
    mGraph.setParty(party);
}

const Coalition &Simulation::getCoalitionByPartyId(int id) const
{
    int selectedIndex = -1;
    for (long unsigned int i = 0; i < mCoalitions.size(); i++)
    {
        Coalition c = mCoalitions.at(i);
        vector<int> ids = c.getIdsVector();

        if (std::find(ids.begin(), ids.end(), id) != ids.end())
        {
            selectedIndex = i;
        }
    }

    return mCoalitions.at(selectedIndex);
}

const Coalition &Simulation::getCoalitionById(int id) const
{
    return mCoalitions.at(id);
}

void Simulation::setCoalition(Coalition &c)
{
    mCoalitions.at(c.getCoalitionId()) = c;
}

/// This method returns a "coalition" vector, where each element is a vector of party IDs in the coalition.
/// At the simulation initialization - the result will be [[agent0.partyId], [agent1.partyId], ...]
const vector<vector<int>> Simulation::getPartiesByCoalitions() const
{
    vector<vector<int>> coalitions = {};
    for (Coalition c : mCoalitions)
    {
        coalitions.push_back(c.getIdsVector());
    }
    // TODO: you MUST implement this method for getting proper output, read the documentation above.
    return coalitions;
}
