#include "Player.h"
#include "../map/Map.h"
#include "../cards/Cards.h"
#include "../orders/Orders.h"
#include "../utils/Utils.h"

using std::endl;
using cris_utils::vectorContains;
using cris_utils::removeElement;

Player::Player(int id, string name)
        : id{id},
          name{name},
          ownedTerritories{vector<int>()},
          allies{vector<int>()},
          hand{new Hand()},
          orders{new OrdersList()} {}

Player::Player(const Player &other)
        : id{other.id},
          name{other.name},
          ownedTerritories{vector<int>(other.ownedTerritories)},
          allies{vector<int>(other.allies)},
          hand{new Hand(*other.hand)},
          orders{new OrdersList()} {}

void swap(Player &a, Player &b) {
    using std::swap;

    swap(a.id, b.id);
    swap(a.name, b.name);
    swap(a.ownedTerritories, b.ownedTerritories);
    swap(a.allies, b.allies);
    swap(a.hand, b.hand);
    swap(a.orders, b.orders);
}

Player &Player::operator=(Player other) {
    swap(*this, other);
    return *this;
}

ostream &operator<<(ostream &out, const Player &obj) {
    out << "Player{" << endl
        << "id: " << obj.id << endl
        << "name: \"" << obj.name << "\"" << endl
        << "ownedTerritories[" << obj.ownedTerritories.size() << "][ ";
    for (int const territory : obj.ownedTerritories) {
        out << territory << " ";
    }
    out << "] " << endl
        << "allies[" << obj.allies.size() << "][ ";
    for (int const ally : obj.allies) {
        out << ally << " ";
    }
    out << "] " << endl
        << "hand: " << *obj.hand << endl
        << "orders: " << *obj.orders << endl
        << "}" << endl;
    return out;
}

vector<int> Player::toDefend() {
    // Temporary, returns arbitrary list of territories
    vector<int> toDefend{};
    for (int i = 0; i < ownedTerritories.size(); i = i + 2) {
        toDefend.push_back(ownedTerritories[i]);
    }
    return toDefend;
}

vector<int> Player::toAttack() {
    // Temporary, returns arbitrary list of territories
    vector<int> toAttack{0, 1};
    return toAttack;
}

void Player::issueOrder(int armies, int territory) {
    orders->add(new DeployOrder(armies, territory));
}

void Player::issueOrder(int armies, int origin, int dest) {
    orders->add(new AdvanceOrder(armies, origin, dest));
}

void Player::issueOrder(Deck *deck, Hand * hand, Card *card, int armies, int origin, int dest, int targetPlayer) {
    orders->add(card->play(deck, hand, origin, dest, armies, targetPlayer));
}

void Player::addTerritory(int territory) {
    ownedTerritories.push_back(territory);
}

void Player::removeTerritory(int territory) {
    removeElement(ownedTerritories, territory);
}

bool Player::owns(int territory) {
    return vectorContains(ownedTerritories, territory);
}


void Player::addAlly(int otherPlayer) {
    allies.push_back(otherPlayer);
}

void Player::addCardOrder(Order *order) {
    orders->add(order);
}

const int &Player::getId() const {
    return id;
}

const vector<int> &Player::getOwnedTerritories() const {
    return ownedTerritories;
}

Hand *Player::getHand() {
    return hand;
}

Player::~Player() {
    delete hand;
    hand = nullptr;

    delete orders;
    orders = nullptr;
}











