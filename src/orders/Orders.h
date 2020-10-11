#pragma once

#include <vector>
#include <iostream>
#include <string>

using std::ostream;
using std::string;
using std::vector;

class Order;
class Territory;
class Map;
class Player;

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
    Order();
    Order(const Order &other);
    friend void swap(Order &a, Order &b);
    friend ostream &operator<<(ostream &out, Order &obj);
    // Can't override assignment operator for abstract class
    // Order &operator=(Order other);

    virtual bool validate(Map *map, Player *player) = 0;
    virtual void execute(Map *map, Player *player) = 0;
    virtual string toString() = 0;

    bool getExecuted() const;
    void setExecuted(bool executed);

    ~Order();
};

class DeployOrder : public Order {
private:
    int armies;
    int territoryId;
public:
    DeployOrder(int armies,
                int territoryId);
    DeployOrder(const DeployOrder &other);
    friend void swap(DeployOrder &a, DeployOrder &b);
    DeployOrder &operator=(DeployOrder other);

    bool validate(Map *map, Player *player) override;
    void execute(Map *map, Player *player) override;
    string toString() override;

    ~DeployOrder();
};

class AdvanceOrder : public Order {
private:
    int armies;
    int originId;
    int destId;
public:
    AdvanceOrder(int armies,
                 int originId,
                 int destId);
    AdvanceOrder(const AdvanceOrder &other);
    friend void swap(AdvanceOrder &a, AdvanceOrder &b);
    AdvanceOrder &operator=(AdvanceOrder other);

    bool validate(Map *map, Player *player) override;
    void execute(Map *map, Player *player) override;
    string toString() override;

    ~AdvanceOrder();
};

class BombOrder : public Order {
private:
    int territoryId;
public:
    BombOrder(int territoryId);
    BombOrder(const BombOrder &other);
    friend void swap(BombOrder &a, BombOrder &b);
    BombOrder &operator=(BombOrder other);

    bool validate(Map *map, Player *player) override;
    void execute(Map *map, Player *player) override;
    string toString() override;

    ~BombOrder();
};

class BlockadeOrder : public Order {
private:
    int territoryId;
public:
    BlockadeOrder(int territoryId);
    BlockadeOrder(const BlockadeOrder &other);
    friend void swap(BlockadeOrder &a, BlockadeOrder &b);
    BlockadeOrder &operator=(BlockadeOrder other);

    bool validate(Map *map, Player *player) override;
    void execute(Map *map, Player *player) override;
    string toString() override;

    ~BlockadeOrder();
};

class AirliftOrder : public Order {
private:
    int armies;
    int originId;
    int destId;
public:
    AirliftOrder(int armies,
                 int originId,
                 int destId);
    AirliftOrder(const AirliftOrder &other);
    friend void swap(AirliftOrder &a, AirliftOrder &b);
    AirliftOrder &operator=(AirliftOrder other);

    bool validate(Map *map, Player *player) override;
    void execute(Map *map, Player *player) override;
    string toString() override;

    ~AirliftOrder();
};

class NegotiateOrder : public Order {
private:
    int playerId;
public:
    NegotiateOrder(int playerId);
    NegotiateOrder(const NegotiateOrder &other);
    friend void swap(NegotiateOrder &a, NegotiateOrder &b);
    NegotiateOrder &operator=(NegotiateOrder other);

    bool validate(Map *map, Player *player) override;
    void execute(Map *map, Player *player) override;
    string toString() override;

    ~NegotiateOrder();
};

