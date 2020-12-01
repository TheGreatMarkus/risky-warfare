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

/*
 * Note that Order and its subclasses aren't responsible for managing the
 * memory of the game state. Therefore, the default copy constructor,
 * assignment operator and destructor are used, despite the presence of
 * pointer attributes
 */
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
    friend ostream &operator<<(ostream &out, const Order &obj);

    virtual bool validate(Map *map, Player *player) = 0;
    virtual void execute(Map *map, Player *player) = 0;
    virtual void print(ostream &out) const = 0;
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

    bool validate(Map *map, Player *player) override;
    void execute(Map *map, Player *player) override;
    void print(ostream &out) const override;
    DeployOrder *clone() override;
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

    bool validate(Map *map, Player *player) override;
    void execute(Map *map, Player *player) override;
    void print(ostream &out) const override;
    AdvanceOrder *clone() override;

    Territory *getDest() const;
};

/**
 * Order for bombing territories
 */
class BombOrder : public Order {
private:
    Territory *territory;
public:
    BombOrder(Territory *territory);

    bool validate(Map *map, Player *player) override;
    void execute(Map *map, Player *player) override;
    void print(ostream &out) const override;
    BombOrder *clone() override;
};

/**
 * Order for creating blockades
 */
class BlockadeOrder : public Order {
private:
    Territory *territory;
public:
    BlockadeOrder(Territory *territory);

    bool validate(Map *map, Player *player) override;
    void execute(Map *map, Player *player) override;
    void print(ostream &out) const override;
    BlockadeOrder *clone() override;
};

/**
 * Order for moving troops to any territory
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

    bool validate(Map *map, Player *player) override;
    void execute(Map *map, Player *player) override;
    void print(ostream &out) const override;
    AirliftOrder *clone() override;
};

/**
 * Order for negotiating with other players
 */
class NegotiateOrder : public Order {
private:
    Player *player;
public:
    NegotiateOrder(Player *player);

    bool validate(Map *map, Player *player) override;
    void execute(Map *map, Player *player) override;
    void print(ostream &out) const override;
    NegotiateOrder *clone() override;
};

