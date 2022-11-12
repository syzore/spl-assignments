#include "Party.h"
#include "Agent.h"
#include "JoinPolicy.h"
#include "Simulation.h"

Party::Party(int id, string name, int mandates, JoinPolicy *jp) : mId(id), mName(name), mMandates(mandates), mJoinPolicy(jp), mState(Waiting)
{
    timer = 3;
}

State Party::getState() const
{
    return mState;
}

void Party::setState(State state)
{
    mState = state;
}

int Party::getMandates() const
{
    return mMandates;
}

const string &Party::getName() const
{
    return mName;
}

void Party::step(Simulation &s)
{
    if (mState == CollectingOffers)
    {
        timer--;
        if (timer == 0)
        {

            (*mJoinPolicy).join();

            vector<vector<int>> partiesByCoalitions = s.getPartiesByCoalitions();
            int numOfCoalition = partiesByCoalitions.size();

            vector<Agent> agents = s.getAgents();

            // lock offer
        }
    }
}
