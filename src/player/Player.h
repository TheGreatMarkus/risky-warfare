#pragma once

#include <vector>
#include <string>
#include <iostream>
#include <set>
#include "../map/Map.h"
#include "../GameEngine.h"

using std::ostream;
using std::string;
using std::vector;
using std::set;

class Territory;
class Hand;
class Order;
class OrdersList;
class Card;
class Deck;

enum class OrderType;

/**
 * Class representing the player in the game
 */
class Player {
private:
    string name;
    int armies;
    bool cardDue;
    set<Territory *> ownedTerritories;
    set<Player *> allies;
    Hand *hand;
    OrdersList *orders;
public:
    Player(string name);
    Player(const Player &other);
    friend void swap(Player &a, Player &b);
    Player &operator=(Player other);
    friend ostream &operator<<(ostream &out, const Player &obj);

    vector<Territory *> toDefend(Map *map);
    vector<Territory *> toAttack(Map *map);
    void issueOrder(Map *map, Deck *deck, vector<Player *> activePlayers);
    void issueDeployOrder(Map *map);
    void issueAdvanceOrder(Map *map,
                           string verb,
                           vector<Territory *> list);
    void captureTerritory(Territory *territory);
    void loseTerritory(Territory *territory);
    bool owns(Territory *territory);
    void addAlly(Player *otherPlayer);
    void resetAllies();
    const vector<Territory *> getNeighboringTerritories(Map *map) const;

    const string &getName() const;
    const int &getArmies() const;
    bool isCardDue() const;
    const set<Territory *> &getOwnedTerritories() const;
    Hand *getHand();
    OrdersList *getOrders() const;
    const set<Player *> &getAllies() const;

    void addArmies(int armies);
    void removeArmies(int armies);
    void setCardDue(bool cardDue);

    ~Player();

};
