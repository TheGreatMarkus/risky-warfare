#pragma once

#include <vector>
#include <string>
#include <iostream>
#include <set>

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

    vector<Territory *> toDefend();
    vector<Territory *> toAttack();
    void issueOrder();
    void issueOrder(int armies,
                    Territory *territory);
    void issueOrder(int armies,
                    Territory *origin,
                    Territory *dest);
    void issueOrder(Deck *deck,
                    Card *card,
                    int armies,
                    Territory *origin,
                    Territory *dest,
                    Player *targetPlayer);
    void captureTerritory(Territory *territory);
    void loseTerritory(Territory *territory);
    bool owns(Territory *territory);
    void addAlly(Player *otherPlayer);
    void addCardOrder(Order *order);

    const string &getName() const;
    const int & getArmies() const;
    OrdersList &getOrders() ;
    const set<Territory *> &getOwnedTerritories() const;
    Hand *getHand();

    void addArmies(int armies);
    void removeArmies(int armies);

    ~Player();

};