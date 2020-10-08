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

class Player {
private:
    int id;
    string name;
    vector<Territory *> *ownedTerritories;
    Hand *hand;
    OrdersList *orders;

public:
    Player(int id, string name_);

    Player(const Player &other);

    Player &operator=(Player other);

    friend void swap(Player &a, Player &b);

    friend ostream &operator<<(ostream &out, const Player &obj);

    vector<Territory *> toDefend();

    vector<Territory *> toAttack();

    void issueOrder(); // create Order

    ~Player();
};
