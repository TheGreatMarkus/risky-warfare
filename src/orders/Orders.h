#pragma once

#include <vector>
#include <iostream>
#include <string>

using std::ostream;
using std::string;
using std::vector;
using std::stringstream;

class Order;
class Territory;
class Map;
class Player;


enum class OrderType {
    DEPLOY,
    ADVANCE,
    BOMB,
    BLOCKADE,
    AIRLIFT,
    NEGOTIATE,
};

/**
 * Class for representing a list of orders
 */
class OrdersList {
private:
    vector<Order *> orders;

public:
    OrdersList();
    OrdersList(const OrdersList &other);
    friend void swap(OrdersList &a, OrdersList &b);
    OrdersList &operator=(OrdersList other);
    friend ostream &operator<<(ostream &out, const OrdersList &obj);

    Order *operator[](int i);
    const int size() const;
    const bool empty() const;
    void add(Order *order);
    void move(int origin, int dest);
    void remove(int i);
    void remove(Order *order);
    Order *getHighestPriorityOrder();

    ~OrdersList();
};

/**
 * Abstract class for player orders.
 */
class Order {
private:
    bool executed;
    string effect;
    OrderType type;
public:
    Order(OrderType type);
    Order(const Order &other);
    friend void swap(Order &a, Order &b);
    // abstract class, can't overload assignment operator
    // Order &operator=(Order other);
    friend ostream &operator<<(ostream &out, const Order &obj);

    virtual bool validate(Map *map, Player *player) = 0;
    virtual void execute(Map *map, Player *player) = 0;
    virtual ostream &print(ostream &out) const = 0;
    virtual Order *clone() = 0;

    const bool &isExecuted() const;
    const string &getEffect() const;
    OrderType getType() const;

    void setExecuted(bool executed);
    void setEffect(string effect);


    virtual ~Order();
};

/**
 * Order for deploying troops.
 */
class DeployOrder : public Order {
private:
    int armies;
    Territory *territory;
public:
    DeployOrder(int armies,
                Territory *territory);
    DeployOrder(const DeployOrder &other);
    friend void swap(DeployOrder &a, DeployOrder &b);
    DeployOrder &operator=(DeployOrder other);

    bool validate(Map *map, Player *player) override;
    void execute(Map *map, Player *player) override;
    ostream &print(ostream &out) const override;
    DeployOrder *clone() override;

    ~DeployOrder() override;
};

/**
 * Order for advancing troops
 */
class AdvanceOrder : public Order {
private:
    int armies;
    Territory *origin;
    Territory *dest;
public:
    AdvanceOrder(int armies,
                 Territory *originTerr,
                 Territory *destTerr);
    AdvanceOrder(const AdvanceOrder &other);
    friend void swap(AdvanceOrder &a, AdvanceOrder &b);
    AdvanceOrder &operator=(AdvanceOrder other);

    bool validate(Map *map, Player *player) override;
    void execute(Map *map, Player *player) override;
    ostream &print(ostream &out) const override;
    AdvanceOrder *clone() override;

    ~AdvanceOrder() override;
};

/**
 * Order for bombing territories
 */
class BombOrder : public Order {
private:
    Territory *territory;
public:
    BombOrder(Territory *territory);
    BombOrder(const BombOrder &other);
    friend void swap(BombOrder &a, BombOrder &b);
    BombOrder &operator=(BombOrder other);

    bool validate(Map *map, Player *player) override;
    void execute(Map *map, Player *player) override;
    ostream &print(ostream &out) const override;
    BombOrder *clone() override;

    ~BombOrder() override;
};

/**
 * Order for creating blockades
 */
class BlockadeOrder : public Order {
private:
    Territory *territory;
public:
    BlockadeOrder(Territory *territory);
    BlockadeOrder(const BlockadeOrder &other);
    friend void swap(BlockadeOrder &a, BlockadeOrder &b);
    BlockadeOrder &operator=(BlockadeOrder other);

    bool validate(Map *map, Player *player) override;
    void execute(Map *map, Player *player) override;
    ostream &print(ostream &out) const override;
    BlockadeOrder *clone() override;

    ~BlockadeOrder() override;
};

/**
 * Order for moving troops long distances
 */
class AirliftOrder : public Order {
private:
    int armies;
    Territory *origin;
    Territory *dest;
public:
    AirliftOrder(int armies,
                 Territory *origin,
                 Territory *dest);
    AirliftOrder(const AirliftOrder &other);
    friend void swap(AirliftOrder &a, AirliftOrder &b);
    AirliftOrder &operator=(AirliftOrder other);

    bool validate(Map *map, Player *player) override;
    void execute(Map *map, Player *player) override;
    ostream &print(ostream &out) const override;
    AirliftOrder *clone() override;

    ~AirliftOrder() override;
};

/**
 * Order for negotiating with other players
 */
class NegotiateOrder : public Order {
private:
    Player *player;
public:
    NegotiateOrder(Player *player);
    NegotiateOrder(const NegotiateOrder &other);
    friend void swap(NegotiateOrder &a, NegotiateOrder &b);
    NegotiateOrder &operator=(NegotiateOrder other);

    bool validate(Map *map, Player *player) override;
    void execute(Map *map, Player *player) override;
    ostream &print(ostream &out) const override;
    NegotiateOrder *clone() override;

    ~NegotiateOrder() override;
};

