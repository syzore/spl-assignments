#pragma once

class JoinPolicy
{
public:
    JoinPolicy();
    void join();
};

class MandatesJoinPolicy : public JoinPolicy
{
public:
    MandatesJoinPolicy();
    void join();
};

class LastOfferJoinPolicy : public JoinPolicy
{
public:
    LastOfferJoinPolicy();
    void join();
};