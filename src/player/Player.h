#pragma once

#include <vector>
#include <string>
#include <iostream>

using std::ostream;
using std::string;
using std::vector;

class Territory;
class Hand;
class Order;
class OrdersList;
class Card;
class Deck;

enum class OrderType;

class Player {
private:
    int id;
    string name;
    vector<int> ownedTerritories;
    vector<int> allies;
    Hand *hand;
    OrdersList *orders;
public:

    Player(int id, string name);
    Player(const Player &other);

    friend void swap(Player &a, Player &b);
    Player &operator=(Player other);
    friend ostream &operator<<(ostream &out, const Player &obj);

    vector<int> toDefend();
    vector<int> toAttack();
    void issueOrder(int armies,
                    int territory);
    void issueOrder(int armies,
                    int origin,
                    int dest);
    void issueOrder(Deck *deck,
                    Hand * hand,
                    Card * card,
                    int armies,
                    int origin,
                    int dest,
                    int targetPlayer);
    void addTerritory(int territory);
    void removeTerritory(int territory);
    bool owns(int territory);
    void addAlly(int otherPlayer);
    void addCardOrder(Order *order);

    const int &getId() const;
    const vector<int> &getOwnedTerritories() const;
    Hand *getHand();

    ~Player();

};