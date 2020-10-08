#pragma once

#include <vector>
#include <iostream>
#include <string>

using std::ostream;
using std::string;
using std::vector;

class Order;
class Territory;

class OrdersList {
private:
    vector<Order *> orders;

public:
    OrdersList(/* args */);
    OrdersList(const OrdersList &other);
    friend void swap(OrdersList &a, OrdersList &b);
    OrdersList &operator=(OrdersList other);
    friend ostream &operator<<(ostream &out, const OrdersList &obj);

    bool moveOrder(int origin, int dest);
    bool deleteOrder(int i);

    ~OrdersList();
};

class Order {
private:
    bool executed;
public:
    Order(bool executed);
    Order(const Order &other);
    friend void swap(Order &a, Order &b);
    Order &operator=(Order other);
    friend ostream &operator<<(ostream &out, Order &obj);

    virtual bool validate() = 0;
    virtual void execute() = 0;
    virtual string toString() = 0;

    bool getExecuted() const;
    void setExecuted(bool executed);

    ~Order();
};

class DeployOrder : public Order {
private:
    int armies;
    int originId;
    int destId;
public:
    DeployOrder(bool executed,
                int armies,
                int originId,
                int destId;
    DeployOrder(const DeployOrder &other);
    friend void swap(DeployOrder &a, DeployOrder &b);
    DeployOrder &operator=(DeployOrder other);

    bool validate() override;
    void execute() override;
    string toString() override;

    ~DeployOrder();
};

class AdvanceOrder : public Order {
private:
    /* data */
public:
    AdvanceOrder(/* args */);

    bool validate();

    void execute();

    string toString();

    ~AdvanceOrder();
};

class BombOrder : public Order {
private:
    /* data */
public:
    BombOrder(/* args */);

    bool validate();

    void execute();

    string toString();

    ~BombOrder();
};

class BlockadeOrder : public Order {
private:
    /* data */
public:
    BlockadeOrder(/* args */);

    bool validate();

    void execute();

    string toString();

    ~BlockadeOrder();
};

class AirliftOrder : public Order {
private:
    /* data */
public:
    AirliftOrder(/* args */);

    bool validate();

    void execute();

    string toString();

    ~AirliftOrder();
};

class NegotiateOrder : public Order {
private:
    /* data */
public:
    NegotiateOrder(/* args */);

    bool validate();

    void execute();

    string toString();

    ~NegotiateOrder();
};

