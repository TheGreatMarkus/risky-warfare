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

/**
 * Abstract class used as the interface for the Strategy pattern
 */
class PlayerStrategy {
protected:
    Player *player;
public:
    PlayerStrategy(Player *player);
    friend ostream &operator<<(ostream &out, const PlayerStrategy &obj);

    virtual bool issueOrder(Map *map,
                            Deck *deck,
                            vector<Player *> activePlayers) = 0;
    virtual vector<Territory *> toAttack(Map *map) = 0;
    virtual vector<Territory *> toDefend(Map *map) = 0;
    virtual PlayerStrategy *clone() = 0;
    virtual void print(ostream &out) const = 0;

    virtual ~PlayerStrategy();
};

/**
 * PlayerStrategy implementation which uses user input in decision making
 */
class HumanPlayerStrategy : public PlayerStrategy {
private:
    void fillInAdvanceOrder(Map *map,
                            Territory *&origin,
                            Territory *&dest,
                            int &armies,
                            string verb,
                            vector<Territory *> list);
public:
    HumanPlayerStrategy(Player *player);

    bool issueOrder(Map *map,
                    Deck *deck,
                    vector<Player *> activePlayers) override;
    vector<Territory *> toAttack(Map *map) override;
    vector<Territory *> toDefend(Map *map) override;
    PlayerStrategy *clone() override;
    void print(ostream &out) const override;

};

/**
 * PlayerStrategy implementation which is focused on attack
 */
class AggressivePlayerStrategy : public PlayerStrategy {
public:
    AggressivePlayerStrategy(Player *player);

    bool issueOrder(Map *map,
                    Deck *deck,
                    vector<Player *> activePlayers) override;
    vector<Territory *> toAttack(Map *map) override;
    vector<Territory *> toDefend(Map *map) override;
    PlayerStrategy *clone() override;
    void print(ostream &out) const override;
};

/**
 * PlayerStrategy implementation which is focused on defending weak countries
 */
class BenevolentPlayerStrategy : public PlayerStrategy {
public:
    BenevolentPlayerStrategy(Player *player);

    bool issueOrder(Map *map,
                    Deck *deck,
                    vector<Player *> activePlayers) override;
    vector<Territory *> toAttack(Map *map) override;
    vector<Territory *> toDefend(Map *map) override;
    PlayerStrategy *clone() override;
    void print(ostream &out) const override;
};

/**
 * PlayerStrategy implementation which does no issue others
 */
class NeutralPlayerStrategy : public PlayerStrategy {
public:
    NeutralPlayerStrategy(Player *player);

    bool issueOrder(Map *map,
                    Deck *deck,
                    vector<Player *> activePlayers) override;
    vector<Territory *> toAttack(Map *map) override;
    vector<Territory *> toDefend(Map *map) override;
    PlayerStrategy *clone() override;
    void print(ostream &out) const override;
};
