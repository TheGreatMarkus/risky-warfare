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

class OrdersList {
private:
    vector<Order *> orders;

public:
    OrdersList(/* args */);
    OrdersList(const OrdersList &other);
    friend void swap(OrdersList &a, OrdersList &b);
    OrdersList &operator=(OrdersList other);
    friend ostream &operator<<(ostream &out, const OrdersList &obj);

    void add(Order *order);
    void move(int origin, int dest);
    void remove(int i);

    virtual ~OrdersList();
};

class Order {
private:
    bool executed;
    string effect;
public:
    Order();
    Order(const Order &other);
    friend void swap(Order &a, Order &b);
    //Order &operator=(Order other); // abstract class, can't overload assignment operator
    friend ostream &operator<<(ostream &out, const Order &obj);

    virtual bool validate(Map *map, Player *player) = 0;
    virtual void execute(Map *map, Player *player) = 0;
    virtual ostream &print(ostream &out) const = 0;
    virtual Order *clone() = 0;

    const bool &getExecuted() const;
    const string &getEffect() const;
    void setExecuted(bool executed);
    void setEffect(string effect);


    virtual ~Order();
};

class DeployOrder : public Order {
private:
    int armies;
    int territory;
public:
    DeployOrder(int armies,
                int territory);
    DeployOrder(const DeployOrder &other);
    friend void swap(DeployOrder &a, DeployOrder &b);
    DeployOrder &operator=(DeployOrder other);
    friend ostream &operator<<(ostream &out, const DeployOrder &obj);

    bool validate(Map *map, Player *player) override;
    void execute(Map *map, Player *player) override;
    ostream &print(ostream &out) const override;
    DeployOrder *clone() override;

    ~DeployOrder();
};

class AdvanceOrder : public Order {
private:
    int armies;
    int origin;
    int dest;
public:
    AdvanceOrder(int armies,
                 int originTerr,
                 int destTerr);
    AdvanceOrder(const AdvanceOrder &other);
    friend void swap(AdvanceOrder &a, AdvanceOrder &b);
    AdvanceOrder &operator=(AdvanceOrder other);
    friend ostream &operator<<(ostream &out, const AdvanceOrder &obj);

    bool validate(Map *map, Player *player) override;
    void execute(Map *map, Player *player) override;
    ostream &print(ostream &out) const override;
    AdvanceOrder *clone() override;

    ~AdvanceOrder();
};

class BombOrder : public Order {
private:
    int territory;
public:
    BombOrder(int territory);
    BombOrder(const BombOrder &other);
    friend void swap(BombOrder &a, BombOrder &b);
    BombOrder &operator=(BombOrder other);
    friend ostream &operator<<(ostream &out, const BombOrder &obj);

    bool validate(Map *map, Player *player) override;
    void execute(Map *map, Player *player) override;
    ostream &print(ostream &out) const override;
    BombOrder *clone() override;

    ~BombOrder();
};

class BlockadeOrder : public Order {
private:
    int territory;
public:
    BlockadeOrder(int territory);
    BlockadeOrder(const BlockadeOrder &other);
    friend void swap(BlockadeOrder &a, BlockadeOrder &b);
    BlockadeOrder &operator=(BlockadeOrder other);
    friend ostream &operator<<(ostream &out, const BlockadeOrder &obj);

    bool validate(Map *map, Player *player) override;
    void execute(Map *map, Player *player) override;
    ostream &print(ostream &out) const override;
    BlockadeOrder *clone() override;

    ~BlockadeOrder();
};

class AirliftOrder : public Order {
private:
    int armies;
    int origin;
    int dest;
public:
    AirliftOrder(int armies,
                 int origin,
                 int dest);
    AirliftOrder(const AirliftOrder &other);
    friend void swap(AirliftOrder &a, AirliftOrder &b);
    AirliftOrder &operator=(AirliftOrder other);
    friend ostream &operator<<(ostream &out, const AirliftOrder &obj);

    bool validate(Map *map, Player *player) override;
    void execute(Map *map, Player *player) override;
    ostream &print(ostream &out) const override;
    AirliftOrder *clone() override;

    ~AirliftOrder();
};

class NegotiateOrder : public Order {
private:
    int player;
public:
    NegotiateOrder(int playerId);
    NegotiateOrder(const NegotiateOrder &other);
    friend void swap(NegotiateOrder &a, NegotiateOrder &b);
    NegotiateOrder &operator=(NegotiateOrder other);
    friend ostream &operator<<(ostream &out, const NegotiateOrder &obj);

    bool validate(Map *map, Player *player) override;
    void execute(Map *map, Player *player) override;
    ostream &print(ostream &out) const override;
    NegotiateOrder *clone() override;

    ~NegotiateOrder();
};

