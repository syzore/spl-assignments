#pragma once

class JoinPolicy
{
public:
    JoinPolicy(){};
    virtual void join() = 0;
};

class MandatesJoinPolicy : public JoinPolicy
{
public:
    MandatesJoinPolicy() = default;
    void join();
};

class LastOfferJoinPolicy : public JoinPolicy
{
public:
    LastOfferJoinPolicy() = default;
    void join();
};