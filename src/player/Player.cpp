#include "Player.h"

#include "../cards/Cards.h"
#include "../orders/Orders.h"
#include "../utils/Utils.h"
#include "../map/Map.h"

using std::endl;
using std::cout;
using std::to_string;

using cris_utils::contains;
using cris_utils::removeElement;
using cris_utils::getIntInput;


Player::Player(string name)
        : name{name},
          ownedTerritories{set<Territory *>()},
          allies{set<Player *>()},
          hand{new Hand()},
          orders{new OrdersList()} {}

Player::Player(const Player &other)
        : name{other.name},
          ownedTerritories{set<Territory *>(other.ownedTerritories)},
          allies{set<Player *>(other.allies)},
          hand{new Hand(*other.hand)},
          orders{new OrdersList()} {}

/**
 * Swap method for copy-and-swap
 * @param a first element
 * @param b second element
 */
void swap(Player &a, Player &b) {
    using std::swap;

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
        << "name: \"" << obj.name << "\"" << endl
        << "ownedTerritories[" << obj.ownedTerritories.size() << "][ ";
    for (auto territory : obj.ownedTerritories) {
        out << territory->getName() << " ";
    }
    out << "] " << endl
        << "allies[" << obj.allies.size() << "][ ";
    for (auto ally : obj.allies) {
        out << ally->getName() << " ";
    }
    out << "] " << endl
        << "hand: " << *obj.hand << endl
        << "orders: " << *obj.orders << endl
        << "}" << endl;
    return out;
}

/**
 * Returns list of territories to defend
 */
vector<Territory *> Player::toDefend() {
    // TODO, currently returns arbitrary list of territories
    vector<Territory *> toDefend{};
    return toDefend;
}

/**
 * Returns list of territories to attack
 */
vector<Territory *> Player::toAttack() {
    // TODO, currently returns arbitrary list of territories
    vector<Territory *> toAttack{};
    return toAttack;
}

void Player::issueOrder() {
    cout << "Player " << name << " is issuing orders!" << endl;

    vector<Territory *> attack = toAttack();
    vector<Territory *> defend = toDefend();

    // TODO issue deploy and advance orders

    cout << "Player " << name << ", you have the following cards: " << endl;
    for (int i = 0; i < hand->getCards().size(); ++i) {
        cout << i << ": " << *(hand->getCards()[i]) << endl;
    }
    int cardIndex = getIntInput("Pick a card", 0, hand->getCards().size() - 1);


}

/**
 * Issue DeployOrder
 * @param armies
 * @param territory
 */
void Player::issueOrder(int armies, Territory *territory) {
    orders->add(new DeployOrder(armies, territory));
}

/**
 * Issue AdvanceOrder
 * @param armies
 * @param territory
 */
void Player::issueOrder(int armies, Territory *origin, Territory *dest) {
    orders->add(new AdvanceOrder(armies, origin, dest));
}

/**
 * Issue Order from a Card
 * @param armies
 * @param territory
 */
void Player::issueOrder(Deck *deck, Card *card, int armies, Territory *origin, Territory *dest, Player *targetPlayer) {
    orders->add(card->play(deck, hand, origin, dest, armies, targetPlayer));
}

void Player::captureTerritory(Territory *territory) {
    ownedTerritories.insert(territory);
    territory->setPlayer(this);
}

void Player::loseTerritory(Territory *territory) {
    removeElement(ownedTerritories, territory);
    territory->setPlayer(nullptr);
}

/**
 * Checks whether player own a territory
 * @param territory
 * @return
 */
bool Player::owns(Territory *territory) {
    return contains(ownedTerritories, territory);
}

/**
 * Add ally after negotiation
 * @param otherPlayer
 */
void Player::addAlly(Player *otherPlayer) {
    allies.insert(otherPlayer);
}

void Player::addCardOrder(Order *order) {
    orders->add(order);
}

const string &Player::getName() const {
    return name;
}

const int &Player::getArmies() const {
    return armies;
}

OrdersList &Player::getOrders() {
    return *orders;
}


const set<Territory *> &Player::getOwnedTerritories() const {
    return ownedTerritories;
}

Hand *Player::getHand() {
    return hand;
}

void Player::addArmies(int armies) {
    this->armies += armies;
}

void Player::removeArmies(int armies) {
    if (this->armies < armies) {
        throw "Player" + this->getName() + " doesn't have enough armies: " + to_string(armies);
    }
}

Player::~Player() {
    delete hand;
    hand = nullptr;

    delete orders;
    orders = nullptr;
}





















