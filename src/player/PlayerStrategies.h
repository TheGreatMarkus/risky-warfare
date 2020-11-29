#pragma once

#include <vector>
#include <string>
#include <iostream>

using std::vector;
using std::string;
using std::ostream;

class Player;
class Map;
class Territory;
class Deck;

class PlayerStrategy {
private:
    Player *player;
public:
    PlayerStrategy(Player *player);
    PlayerStrategy(const PlayerStrategy &other);
    friend void swap(PlayerStrategy &a, PlayerStrategy &b);
    // abstract class, can't overload assignment operator
    // PlayerStrategy &operator=(PlayerStrategy other);
    friend ostream &operator<<(ostream &out, const PlayerStrategy &obj);

    virtual void issueOrder(Map *map,
                            Deck *deck,
                            vector<Player *> activePlayers) = 0;
    virtual vector<Territory *> toAttack(Map *map) = 0;
    virtual vector<Territory *> toDefend(Map *map) = 0;
    virtual PlayerStrategy *clone() = 0;
    virtual ostream &print(ostream &out) const = 0;

    virtual ~PlayerStrategy();
};

class HumanPlayerStrategy : public PlayerStrategy {
public:
    HumanPlayerStrategy(Player *player);
    HumanPlayerStrategy(const HumanPlayerStrategy &other);
    friend void swap(HumanPlayerStrategy &a, HumanPlayerStrategy &b);
    HumanPlayerStrategy &operator=(HumanPlayerStrategy other);
    friend ostream &operator<<(ostream &out, const HumanPlayerStrategy &obj);

    void issueOrder(Map *map,
                    Deck *deck,
                    vector<Player *> activePlayers) override;
    vector<Territory *> toAttack(Map *map) override;
    vector<Territory *> toDefend(Map *map) override;
    PlayerStrategy *clone() override;
    ostream &print(ostream &out) const override;

    virtual ~HumanPlayerStrategy();
};

class AggressivePlayerStrategy : public PlayerStrategy {
public:
    AggressivePlayerStrategy(Player *player);
    AggressivePlayerStrategy(const AggressivePlayerStrategy &other);
    friend void swap(AggressivePlayerStrategy &a, AggressivePlayerStrategy &b);
    AggressivePlayerStrategy &operator=(AggressivePlayerStrategy other);
    friend ostream &operator<<(ostream &out, const AggressivePlayerStrategy &obj);

    void issueOrder(Map *map,
                    Deck *deck,
                    vector<Player *> activePlayers) override;
    vector<Territory *> toAttack(Map *map) override;
    vector<Territory *> toDefend(Map *map) override;
    PlayerStrategy *clone() override;
    ostream &print(ostream &out) const override;

    virtual ~AggressivePlayerStrategy();
};

class BenevolentPlayerStrategy : public PlayerStrategy {
public:
    BenevolentPlayerStrategy(Player *player);
    BenevolentPlayerStrategy(const BenevolentPlayerStrategy &other);
    friend void swap(BenevolentPlayerStrategy &a, BenevolentPlayerStrategy &b);
    BenevolentPlayerStrategy &operator=(BenevolentPlayerStrategy other);
    friend ostream &operator<<(ostream &out, const BenevolentPlayerStrategy &obj);

    void issueOrder(Map *map,
                    Deck *deck,
                    vector<Player *> activePlayers) override;
    vector<Territory *> toAttack(Map *map) override;
    vector<Territory *> toDefend(Map *map) override;
    PlayerStrategy *clone() override;
    ostream &print(ostream &out) const override;

    virtual ~BenevolentPlayerStrategy();
};

class NeutralPlayerStrategy : public PlayerStrategy {
public:
    NeutralPlayerStrategy(Player *player);
    NeutralPlayerStrategy(const NeutralPlayerStrategy &other);
    friend void swap(NeutralPlayerStrategy &a, NeutralPlayerStrategy &b);
    NeutralPlayerStrategy &operator=(NeutralPlayerStrategy other);
    friend ostream &operator<<(ostream &out, const NeutralPlayerStrategy &obj);

    void issueOrder(Map *map,
                    Deck *deck,
                    vector<Player *> activePlayers) override;
    vector<Territory *> toAttack(Map *map) override;
    vector<Territory *> toDefend(Map *map) override;
    PlayerStrategy *clone() override;
    ostream &print(ostream &out) const override;

    virtual ~NeutralPlayerStrategy();
};
