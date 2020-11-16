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
    void addTerritory(Territory *territory);
    void removeTerritory(Territory *territory);
    bool owns(Territory *territory);
    void addAlly(Player *otherPlayer);
    void addCardOrder(Order *order);

    const int &getId() const;
    const string &getName() const;
    const set<Territory *> &getOwnedTerritories() const;
    Hand *getHand();

    ~Player();

};