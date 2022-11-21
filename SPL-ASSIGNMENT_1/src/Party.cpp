#include <iostream>

#include "Party.h"
#include "Simulation.h"
#include "JoinPolicy.h"

using std::cout;
using std::endl;

Party::Party(int id, string name, int mandates, JoinPolicy *jp) : mId(id), mName(name), mMandates(mandates), mJoinPolicy(jp), mState(Waiting), timer(3)
{
}

Party::~Party()
{
    mJoinPolicy = nullptr;
    delete mJoinPolicy;
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

void Party::suggest(Agent &agent)
{
    cout << "inside suggest" << endl;

    if (mState == Waiting)
    {
        mState = CollectingOffers;
        cout << "party #" << mId << " changes state to Collecting Offers" << endl;
    }
    cout << "after waiting if" << endl;

    cout << "joinPolicy = " << mJoinPolicy << endl;
    (*mJoinPolicy).addOffer(agent);
}
