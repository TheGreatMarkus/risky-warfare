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

class Player
{
private:
    string name;
    vector<Territory *> *ownedTerritories;
    Hand *hand;
    vector<Order *> *orders;

public:
    Player(string name_);
    Player(const Player &other);

    Player &operator=(Player other);
    friend void swap(Player &first, Player &second);
    friend ostream &operator<<(ostream &out, const Player &obj);

    vector<Territory *> toDefend();
    vector<Territory *> toAttack();
    void issueOrder(); // create Order

    ~Player();
};
