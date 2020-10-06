#include "Player.h"
#include "../map/Map.h"
#include "../cards/Cards.h"
#include "../orders/Orders.h"

Player::Player(string name_)
    : name{name_} {}

Player::Player(const Player &other)
{
}

Player &Player::operator=(Player other)
{
    swap(*this, other);

    return *this;
}

void swap(Player &first, Player &second)
{
}

ostream &operator<<(ostream &out, const Player &obj)
{
    out << "Player{ name: " << obj.name << " }";

    return out;
}

Player::~Player()
{
}