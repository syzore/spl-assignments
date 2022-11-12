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
    void join();
};

class LastOfferJoinPolicy : public JoinPolicy
{
public:
    void join();
};