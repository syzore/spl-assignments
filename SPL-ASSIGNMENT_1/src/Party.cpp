#include "Party.h"
#include "Agent.h"
#include "JoinPolicy.h"
#include "Simulation.h"

// TODO: add a function to party that called Party::suggest(int id) that stores the id's of the agents/agents' party id
// to used after, if the join policy is last offer we take the last id, if the policy is by mendates we can go though the vector
// alognside the coalitions vector and find the coalition with most mandates that is inside the vector.

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
            join(s);
        }
    }
}

void Party::join(Simulation &s)
{
    (*mJoinPolicy).join(*this, s);

    s.reduceAvailableParties();

    setState(Joined);
}

const int Party::getId() const
{
    return mId;
}

void Party::suggest(int partyId)
{
    (*mJoinPolicy).addOffer(partyId);
}
