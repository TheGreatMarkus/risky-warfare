#include "Player.h"
#include "../map/Map.h"
#include "../cards/Cards.h"
#include "../orders/Orders.h"

Player::Player(int id, string name)
        : id{id},
          name{name},
          ownedTerritories{vector<int>()},
          hand{new Hand()},
          orders{new OrdersList()} {}

Player::Player(const Player &other)
        : id{id},
          name{name},
          ownedTerritories{vector<int>(other.ownedTerritories)},
          hand{new Hand(*other.hand)},
          orders{new OrdersList()} {}

void swap(Player &a, Player &b) {
    using std::swap;

    swap(a.id, b.id);
    swap(a.name, b.name);
    swap(a.ownedTerritories, b.ownedTerritories);
    swap(a.hand, b.hand);
    swap(a.orders, b.orders);
}

Player &Player::operator=(Player other) {
    swap(*this, other);
    return *this;
}

ostream &operator<<(ostream &out, const Player &obj) {
    // TODO
    out << "";
    return out;
}

vector<Territory *> Player::toDefend() {
    // TODO return arbitrary list
    return vector<Territory *>();
}

vector<Territory *> Player::toAttack() {
    // TODO return arbitrary list
    return vector<Territory *>();
}

void Player::issueOrder() {

}

Player::~Player() {}


