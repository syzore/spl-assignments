#include <iostream>

#include "Party.h"
#include "Simulation.h"
#include "JoinPolicy.h"

using std::cout;
using std::endl;
using std::move;
using std::vector;

Party::Party(int id, string name, int mandates, JoinPolicy *jp) : mId(id), mName(name), mMandates(mandates), mJoinPolicy(jp), mState(Waiting), timer(3), mOffers()
{
}

Party::Party(const Party &other) : mId(other.mId), mName(other.getName()), mMandates(other.mMandates), mJoinPolicy(), mState(other.mState), timer(other.timer), mOffers(other.mOffers)
{
    mJoinPolicy = other.mJoinPolicy->clone();
}

Party::Party(Party &&other)
{
    mId = other.mId;
    mName = other.getName();
    mMandates = other.mMandates;
    mJoinPolicy = other.mJoinPolicy;
    mState = other.mState;
    timer = other.timer;
    mOffers = other.mOffers;
    other.mJoinPolicy = nullptr;
}

Party &Party::operator=(const Party &other)
{
    if (this != &other)
    {
        mJoinPolicy = other.mJoinPolicy->clone();
        mId = other.mId;
        mName = other.getName();
        mMandates = other.mMandates;
        mState = other.mState;
        timer = other.timer;
        mOffers = other.mOffers;
    }
    return *this;
}

Party &Party::operator=(Party &&other)
{
    if (this != &other)
    {
        mJoinPolicy = other.mJoinPolicy;
        mId = move(other.mId);
        mName = move(other.getName());
        mMandates = move(other.mMandates);
        mState = move(other.mState);
        timer = move(other.timer);
        mOffers = other.mOffers;

        other.mJoinPolicy = nullptr;
    }
    return *this;
}

Party::~Party()
{
    if (mJoinPolicy)
    {
        mJoinPolicy = nullptr;
        delete mJoinPolicy;
    }
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
        s.setParty(*this);
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
    if (mState == Waiting)
    {
        setState(CollectingOffers);
    }
    addOffer(agent);

    cout << "printing number of offers in suggest" << endl;
    printNumberOfOffers();
}

void Party::printNumberOfOffers() const
{
    cout << "join policy num offers = " << getOffersSize() << endl;
}

void Party::addOffer(Agent &agent)
{
    int id = agent.getPartyId();
    mOffers.push_back(id);
}

vector<int> Party::getMOffers()
{
    return mOffers;
}

int Party::getOffersSize() const
{
    return mOffers.size();
}