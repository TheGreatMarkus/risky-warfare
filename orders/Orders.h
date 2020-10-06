#pragma once

#include <vector>
#include <istream>

using namespace std;

class OrdersList;
class Order;

class OrdersList
{
private:
    vector<Order *> *orders;

public:
    OrdersList(/* args */);

    bool moveOrder(int origin, int dest);
    bool deleteOrder(int i);

    ~OrdersList();
};

class Order
{
private:
    /* data */
public:
    Order(/* args */);

    virtual bool validate() = 0;
    virtual void execute() = 0;
    virtual string toString() = 0;

    friend ostream &operator<<(ostream &out, const Order &order);
    ~Order();
};

class DeployOrder : public Order
{
private:
    /* data */
public:
    DeployOrder(/* args */);

    bool validate();
    void execute();
    string toString();

    ~DeployOrder();
};

class AdvanceOrder : public Order
{
private:
    /* data */
public:
    AdvanceOrder(/* args */);

    bool validate();
    void execute();
    string toString();

    ~AdvanceOrder();
};

class BombOrder : public Order
{
private:
    /* data */
public:
    BombOrder(/* args */);

    bool validate();
    void execute();
    string toString();

    ~BombOrder();
};

class BlockadeOrder : public Order
{
private:
    /* data */
public:
    BlockadeOrder(/* args */);

    bool validate();
    void execute();
    string toString();

    ~BlockadeOrder();
};

class AirliftOrder : public Order
{
private:
    /* data */
public:
    AirliftOrder(/* args */);

    bool validate();
    void execute();
    string toString();

    ~AirliftOrder();
};

class NegotiateOrder : public Order
{
private:
    /* data */
public:
    NegotiateOrder(/* args */);

    bool validate();
    void execute();
    string toString();

    ~NegotiateOrder();
};
