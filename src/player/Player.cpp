#include "Player.h"

#include "../cards/Cards.h"
#include "../orders/Orders.h"
#include "../utils/Utils.h"
#include "PlayerStrategies.h"

using std::endl;
using std::cout;
using std::to_string;

using cris_utils::contains;
using cris_utils::removeElement;
using cris_utils::getIntInput;
using cris_utils::getBoolInput;
using cris_utils::setToVector;
using cris_utils::pickFromList;

Player *neutralPlayer = new Player("Neutral Player");

//=============================
// Player Implementation
//=============================

Player::Player(string name)
        : name{name},
          armies{0},
          cardDue{false},
          ownedTerritories{set<Territory *>()},
          allies{set<Player *>()},
          hand{new Hand()},
          orders{new OrdersList()},
          strategy{nullptr} {}

Player::Player(const Player &other)
        : name{other.name},
          armies{other.armies},
          cardDue{other.cardDue},
          ownedTerritories{set<Territory *>(other.ownedTerritories)},
          allies{set<Player *>(other.allies)},
          hand{new Hand(*other.hand)},
          orders{new OrdersList(*other.orders)},
          strategy{other.strategy == nullptr ? nullptr : other.strategy->clone()} {
}

/**
 * Swap method for copy-and-swap
 * @param a first element
 * @param b second element
 */
void swap(Player &a, Player &b) {
    using std::swap;

    swap(a.name, b.name);
    swap(a.armies, b.armies);
    swap(a.cardDue, b.cardDue);
    swap(a.ownedTerritories, b.ownedTerritories);
    swap(a.allies, b.allies);
    swap(a.hand, b.hand);
    swap(a.orders, b.orders);
    swap(a.strategy, b.strategy);
}

Player &Player::operator=(Player other) {
    swap(*this, other);
    return *this;
}

ostream &operator<<(ostream &out, const Player &obj) {
    out << "Player{ name: " << obj.name
        << ", armies: " << obj.armies
        << ", cardDue: " << obj.cardDue
        << ", ownedTerritories[" << obj.ownedTerritories.size() << "]"
        << ", allies[" << obj.allies.size() << "]"
        << ", hand[" << obj.hand->size() << "]"
        << ", orders[" << obj.orders->size() << "]"
        << ", strategy: " << obj.strategy
        << " }";
    return out;
}

/**
 * Returns list of territories to defend
 */
vector<Territory *> Player::toDefend(Map *map) {
    if (strategy == nullptr) {
        cout << name << " - toDefend: strategy not set!" << endl;
        return vector<Territory *>();
    }
    return strategy->toDefend(map);
}

/**
 * Returns list of territories to attack
 */
vector<Territory *> Player::toAttack(Map *map) {
    if (strategy == nullptr) {
        cout << name << " - toAttack: strategy not set!" << endl;
        return vector<Territory *>();
    }
    return strategy->toAttack(map);
}

bool Player::issueOrder(Map *map, Deck *deck, vector<Player *> activePlayers) {
    if (strategy == nullptr) {
        cout << name << " - issueOrder: strategy not set!" << endl;
        return true;
    }
    return strategy->issueOrder(map, deck, activePlayers);
}

/**
 * Issue DeployOrder
 *
 * Assumes that player has enough armies
 * @param armies
 * @param territory
 */
void Player::issueDeployOrder(Territory *territory, int armies) {
    DeployOrder *order = new DeployOrder(armies, territory);
    if (this->armies < armies) {
        cout << "Invalid" << *order << ": Player doesn't have enough armies: " << this->armies;
        delete order;
        return;
    }
    cout << name << " issued " << *order << endl;
    removeArmies(armies);
    territory->addArmies(armies);
    orders->add(order);
}

/**
 * Issue AdvanceOrder
 * @param armies
 * @param territory
 */
void Player::issueAdvanceOrder(Territory *origin, Territory *dest, int armies) {
    if (origin->getAvailableArmies() < armies) {
        cout << "Invalid advance order. "
             << origin->getName() << ": " << origin->getAvailableArmies()
             << ", needed: " << armies;
        return;
    }

    AdvanceOrder *order = new AdvanceOrder(armies, origin, dest);
    origin->reserveArmies(armies);

    cout << name << " issued " << *order << endl;
    orders->add(order);
}

void Player::captureTerritory(Territory *territory) {
    // If territory belonged to a player, remove from their ownedTerritories
    if (territory->getPlayer() != nullptr) {
        removeElement(territory->getPlayer()->ownedTerritories, territory);
    }

    territory->setPlayer(this);

    ownedTerritories.insert(territory);
    cardDue = true;
}

void Player::loseTerritory(Territory *territory) {
    removeElement(ownedTerritories, territory);

    territory->setPlayer(neutralPlayer);

    neutralPlayer->ownedTerritories.insert(territory);
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

void Player::resetAllies() {
    allies.clear();
}

const vector<Territory *> Player::getNeighboringTerritories(Map *map) const {
    vector<Territory *> neighbors{};
    for (auto &territory : map->getTerritories()) {
        if (territory->getPlayer() != this) {
            for (auto &ownedTerritory : ownedTerritories) {
                if (map->areAdjacent(territory, ownedTerritory)) {
                    neighbors.push_back(territory);
                    break;
                }
            }
        }
    }
    return neighbors;
}

const string &Player::getName() const {
    return name;
}

const int &Player::getArmies() const {
    return armies;
}

bool Player::isCardDue() const {
    return cardDue;
}

const set<Territory *> &Player::getOwnedTerritories() const {
    return ownedTerritories;
}

Hand *Player::getHand() {
    return hand;
}

OrdersList *Player::getOrders() const {
    return orders;
}

const set<Player *> &Player::getAllies() const {
    return allies;
}

PlayerStrategy *Player::getStrategy() const {
    return strategy;
}

void Player::addArmies(int armies) {
    this->armies += armies;
}

void Player::removeArmies(int armies) {
    this->armies -= armies;
    if (this->armies < 0) {
        this->armies = 0;
    }
}

void Player::setCardDue(bool cardDue) {
    Player::cardDue = cardDue;
}

void Player::setStrategy(PlayerStrategy *strategy) {
    Player::strategy = strategy;
}

Player::~Player() {
    delete hand;
    delete orders;
    delete strategy;
}











