#include "Player.h"
#include "../map/Map.h"
#include "../cards/Cards.h"
#include "../orders/Orders.h"

Player::Player(int id, string name)
    : id{id},
      name{name} {}

Player::Player(const Player &other)
    : name{other.name} {}

Player &Player::operator=(Player other)
{
    swap(*this, other);
    return *this;
}

void swap(Player &first, Player &second)
{
    using std::swap;

    swap(first.name, second.name);
}

ostream &operator<<(ostream &out, const Player &obj)
{
    out << "Player{ name: " << obj.name << " }";

    return out;
}

Player::~Player()
{
}