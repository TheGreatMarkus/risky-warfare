#include "Orders.h"

#include "../player/Player.h"
#include "../map/Map.h"
#include "../utils/Utils.h"

using std::cout;
using std::endl;
using std::stringstream;
using std::to_string;

using cris_utils::contains;
using cris_utils::randInt;
using cris_utils::removeElement;

namespace {

    const int ATTACK_CHANCE = 60;
    const int DEFEND_CHANCE = 70;

    /**
     * Simulates an invasion attack between two territories
     * @param origin Attacking territory
     * @param attackingArmies armies used by attacker
     * @param dest defending territory
     */
    void attackTerritory(Territory *origin, int attackingArmies, Territory *dest) {
        Player *attacker = origin->getPlayer();
        Player *defender = dest->getPlayer();
        cout << attacker->getName() << " (" << origin->getName()
             << ") is attacking "
             << defender->getName() << " (" << dest->getName() << ")" << endl;
        // Skip if negotiation took place
        if (contains(origin->getPlayer()->getAllies(), dest->getPlayer())) {
            cout << origin->getPlayer()->getName() << " and " << dest->getPlayer()->getName()
                 << " are allies! Skipping attack." << endl;
            origin->addArmies(attackingArmies);
            return;
        }
        // Calculate kills by each side
        int attackerKills = 0;
        int defenderKills = 0;
        for (int i = 0; i < attackingArmies; ++i) {
            int roll = randInt(1, 100);
            if (roll < ATTACK_CHANCE) {
                attackerKills++;
            }
        }

        for (int i = 0; i < dest->getArmies(); ++i) {
            int roll = randInt(1, 100);
            if (roll < DEFEND_CHANCE) {
                defenderKills++;
            }
        }
        cout << "Attacker kills: " << attackerKills << endl;
        cout << "Defender kills: " << defenderKills << endl;

        dest->removeArmies(attackerKills);
        attackingArmies -= defenderKills;
        if (attackingArmies < 0) {
            attackingArmies = 0;
        }

        if (dest->getArmies() == 0 && attackingArmies > 0) {
            // Attack successful
            origin->getPlayer()->captureTerritory(dest);
            dest->removeArmies(dest->getArmies());
            dest->addArmies(attackingArmies);
            cout << "Attack was successful! " << origin->getPlayer()->getName() << " captured "
                 << dest->getName() << endl;
        } else if (dest->getArmies() > 0 && attackingArmies > 0) {
            // Attack failed, origin has some armies left alive
            origin->addArmies(attackingArmies);
            cout << "Attack failed!" << endl;
        } else {
            // Attack failed, origin has no armies left alive, nothing additional happens
            cout << "Attack failed!" << endl;
        }
        cout << "Attacker: " << *origin << endl;
        cout << "Defender: " << *dest << endl;
    }
}

//=============================
// OrdersList Implementation
//=============================

OrdersList::OrdersList() : orders{vector<Order *>()} {}

OrdersList::OrdersList(const OrdersList &other) : orders{vector<Order *>()} {
    for (const auto &order : other.orders) {
        orders.push_back(order->clone());
    }
}

/**
 * Swap method. Used for the copy-and-swap idiom
 *
 * @param a first element
 * @param b second element
 */
void swap(OrdersList &a, OrdersList &b) {
    using std::swap;
    swap(a.orders, b.orders);
}

OrdersList &OrdersList::operator=(OrdersList other) {
    swap(*this, other);
    return *this;
}

ostream &operator<<(ostream &out, const OrdersList &obj) {
    out << "OrdersList{" << endl;
    for (auto order : obj.orders) {
        out << *order << endl;
    }
    out << "}";
    return out;
}

Order *OrdersList::operator[](int i) {
    return orders[i];
}

const int OrdersList::size() const {
    return orders.size();
}

const bool OrdersList::empty() const {
    return orders.empty();
}

void OrdersList::add(Order *order) {
    orders.push_back(order);
}

/**
 * Moves order from origin index to dest index
 *
 * @param origin
 * @param dest
 */
void OrdersList::move(int origin, int dest) {
    if (origin < orders.size() && dest < orders.size()) {
        int finalDest = dest > origin ? dest - 1 : dest;
        Order *tempOrder = orders[origin];

        orders.erase(orders.begin() + origin);
        orders.insert(orders.begin() + finalDest, tempOrder);

    } else {
        cout << "INVALID INDEXES FOR MOVE" << endl;
    }
}

/**
 * Remove order at given index
 *
 * @param i index
 */
void OrdersList::remove(int i) {
    if (orders.size() <= i) {
        cout << "That index doesn't exist in the list!" << endl;
    }
    delete orders[i];
    orders.erase(orders.begin() + i);
}

/**
 * Remove order by value
 *
 * @param order
 */
void OrdersList::remove(Order *order) {
    removeElement(orders, order);
}

/**
 * Returns the highest priority order, according to the following priority:
 * 1. Deploy
 * 2. Airlift
 * 3. Blockade
 * 4. Everything else
 *
 * Returns nullptr when there are no orders
 *
 * @return highest priority order
 */
Order *OrdersList::getHighestPriorityOrder() {
    vector<OrderType> priorities = {OrderType::DEPLOY, OrderType::AIRLIFT, OrderType::BLOCKADE};

    for (auto &priority : priorities) {
        for (const auto &order : orders) {
            if (order->getType() == priority) {
                return order;
            }
        }
    }

    if (!orders.empty()) {
        return orders[0];
    }

    return nullptr;
}

OrdersList::~OrdersList() {
    for (auto order : orders) {
        delete order;
    }
    orders.clear();
}


//=============================
// Order Implementation
//=============================

Order::Order(OrderType type)
        : executed{false},
          effect{""},
          type{type} {}

ostream &operator<<(ostream &out, const Order &obj) {
    obj.print(out);
    return out;
}

const bool &Order::isExecuted() const {
    return executed;
}

const string &Order::getEffect() const {
    return effect;
}

OrderType Order::getType() const {
    return type;
}

void Order::setExecuted(bool executed_) {
    executed = executed_;
}

void Order::setEffect(string effect) {
    this->effect = effect;
}

Order::~Order() {}



//=============================
// DeployedOrder Implementation
//=============================

DeployOrder::DeployOrder(int armies, Territory *territory)
        : Order(OrderType::DEPLOY),
          armies{armies},
          territory{territory} {}

/**
 * Validates current Order
 *
 * @param map
 * @param player
 * @return if order is valid or not
 */
bool DeployOrder::validate(Map *map, Player *player) {
    // Territory must exist
    // Territory must be owned by the player
    if (!contains(map->getTerritories(), territory)
        || !player->owns(territory)) {
        return false;
    }
    return true;
}

/**
 * Executes current order, modifying the game state if needed
 *
 * @param map Current map
 * @param player Player executing the order
 */
void DeployOrder::execute(Map *map, Player *player) {
    if (isExecuted()) {
        cout << *this << " was already executed. Not executing." << endl;
        return;
    }
    if (!validate(map, player)) {
        cout << *this << " is invalid. Not executing." << endl;
        return;
    }
    cout << "Executing " << *this << endl;

    setEffect("Added " + to_string(armies) + " armies to territory " + territory->getName());

    setExecuted(true);
    cout << "Effect: " << getEffect() << endl;
}

/**
 * Helper print function for polymorphic stream insertion
 *
 * @param out
 */
void DeployOrder::print(ostream &out) const {
    out << "DeployOrder{ "
        << "executed: " << isExecuted();
    if (isExecuted()) {
        out << ", effect: " << getEffect();
    } else {
        out << ", armies: " << armies
            << ", territory: " << territory->getName();
    }
    out << " }";
}

/**
 * Helper function for polymorphic cloning
 */
DeployOrder *DeployOrder::clone() {
    return new DeployOrder(*this);
}

//=============================
// AdvanceOrder Implementation
//=============================

AdvanceOrder::AdvanceOrder(int armies, Territory *originTerr, Territory *destTerr)
        : Order(OrderType::ADVANCE),
          armies{armies},
          origin{originTerr},
          dest{destTerr} {}

/**
 * Validates current Order
 *
 * @param map
 * @param player
 * @return if order is valid or not
 */
bool AdvanceOrder::validate(Map *map, Player *player) {
    // both territories must exist
    // Player must own origin territory
    // origin and dest territory must be adjacent
    // origin must have enough armies
    if (!contains(map->getTerritories(), origin)
        || !contains(map->getTerritories(), dest)
        || !player->owns(origin)
        || !map->areAdjacent(origin, dest)
        || origin->getArmies() < armies) {
        return false;
    }

    return true;
}

/**
 * Executes current order, modifying the game state if needed
 *
 * @param map Current map
 * @param player Player executing the order
 */
void AdvanceOrder::execute(Map *map, Player *player) {
    if (isExecuted()) {
        cout << *this << " was already executed. Not executing." << endl;
        return;
    }

    if (!validate(map, player)) {
        cout << *this << " is invalid. Not executing." << endl;
        return;
    }
    cout << "Executing " << *this << endl;

    origin->removeArmies(armies);
    origin->freeArmies(armies);

    if (dest->getPlayer() == player) {
        dest->addArmies(armies);
        setEffect("Moved " + to_string(armies) + " armies from territory "
                  + origin->getName() + " to territory " + dest->getName());
    } else {
        attackTerritory(origin, armies, dest);
        setEffect("Used " + to_string(armies) + " armies from territory "
                  + origin->getName() + " to attack territory " + dest->getName());
    }

    setExecuted(true);
    cout << "Effect: " << getEffect() << endl;

}

/**
 * Helper print function for polymorphic stream insertion
 *
 * @param out
 */
void AdvanceOrder::print(ostream &out) const {
    out << "AdvanceOrder{ "
        << "executed: " << isExecuted();
    if (isExecuted()) {
        out << ", effect: " << getEffect();
    } else {
        out << ", armies: " << armies
            << ", origin: " << origin->getName()
            << ", dest: " << dest->getName();
    }
    out << " }";
}

/**
 * Helper function for polymorphic cloning
 */
AdvanceOrder *AdvanceOrder::clone() {
    return new AdvanceOrder(*this);
}

Territory *AdvanceOrder::getDest() const {
    return dest;
}

//=============================
// BombOrder Implementation
//=============================

BombOrder::BombOrder(Territory *territory)
        : Order(OrderType::BOMB),
          territory{territory} {}

/**
 * Validates current Order
 *
 * @param map
 * @param player
 * @return if order is valid or not
 */
bool BombOrder::validate(Map *map, Player *player) {
    // territory must exist
    // player can't bomb self
    if (!contains(map->getTerritories(), territory)
        || player->owns(territory)) {
        return false;
    }

    // target territory must be adjacent to one of the player's territories
    bool adjacent = false;
    for (const auto &ownedTerritory : player->getOwnedTerritories()) {
        if (map->areAdjacent(ownedTerritory, territory)) {
            adjacent = true;
            break;
        }
    }
    if (!adjacent) {
        return false;
    }

    return true;
}

/**
 * Executes current order, modifying the game state if needed
 *
 * @param map Current map
 * @param player Player executing the order
 */
void BombOrder::execute(Map *map, Player *player) {
    if (isExecuted()) {
        cout << *this << " was already executed. Not executing." << endl;
        return;
    }
    if (!validate(map, player)) {
        cout << *this << " is invalid. Not executing." << endl;
        return;
    }
    cout << "Executing " << *this << endl;

    territory->bomb();
    setEffect("Bombed territory " + territory->getName());

    setExecuted(true);
    cout << "Effect: " << getEffect() << endl;

}

/**
 * Helper print function for polymorphic stream insertion
 *
 * @param out
 */
void BombOrder::print(ostream &out) const {
    out << "BombOrder{ "
        << "executed: " << isExecuted();
    if (isExecuted()) {
        out << ", effect: " << getEffect();
    } else {
        out << ", territory: " << territory->getName();
    }
    out << " }";
}

/**
 * Helper function for polymorphic cloning
 */
BombOrder *BombOrder::clone() {
    return new BombOrder(*this);
}

//=============================
// BlockadeOrder Implementation
//=============================

BlockadeOrder::BlockadeOrder(Territory *territory)
        : Order(OrderType::BLOCKADE),
          territory{territory} {}

/**
 * Validates current Order
 *
 * @param map
 * @param player
 * @return if order is valid or not
 */
bool BlockadeOrder::validate(Map *map, Player *player) {
    // territory must exist and be owned by player
    if (!contains(map->getTerritories(), territory)
        || !player->owns(territory)) {
        return false;
    }

    return true;
}

/**
 * Executes current order, modifying the game state if needed
 *
 * @param map Current map
 * @param player Player executing the order
 */
void BlockadeOrder::execute(Map *map, Player *player) {
    if (isExecuted()) {
        cout << *this << " was already executed. Not executing." << endl;
        return;
    }
    if (!validate(map, player)) {
        cout << *this << " is invalid. Not executing." << endl;
        return;
    }
    cout << "Executing " << *this << endl;

    territory->blockade();
    player->loseTerritory(territory);
    setEffect("Blockaded territory " + territory->getName());

    setExecuted(true);
    cout << "Effect: " << getEffect() << endl;

}

/**
 * Helper print function for polymorphic stream insertion
 *
 * @param out
 */
void BlockadeOrder::print(ostream &out) const {
    out << "BlockadeOrder{ "
        << "executed: " << isExecuted();
    if (isExecuted()) {
        out << ", effect: " << getEffect();
    } else {
        out << ", territory: " << territory->getName();
    }
    out << " }";
}

/**
 * Helper function for polymorphic cloning
 */
BlockadeOrder *BlockadeOrder::clone() {
    return new BlockadeOrder(*this);
}

//=============================
// AirliftOrder Implementation
//=============================

AirliftOrder::AirliftOrder(int armies, Territory *origin, Territory *dest)
        : Order(OrderType::AIRLIFT),
          armies{armies},
          origin{origin},
          dest{dest} {}

/**
 * Validates current Order
 *
 * @param map
 * @param player
 * @return if order is valid or not
 */
bool AirliftOrder::validate(Map *map, Player *player) {
    // Both territories must exist
    // Origin territory must be owned by player
    // Origin must have enough troops to move
    if (!contains(map->getTerritories(), origin)
        || !contains(map->getTerritories(), dest)
        || !player->owns(origin)
        || origin->getArmies() < armies) {
        return false;
    }

    return true;
}

/**
 * Executes current order, modifying the game state if needed
 *
 * @param map Current map
 * @param player Player executing the order
 */
void AirliftOrder::execute(Map *map, Player *player) {
    if (isExecuted()) {
        cout << *this << " was already executed. Not executing." << endl;
        return;
    }
    if (!validate(map, player)) {
        cout << *this << " is invalid. Not executing." << endl;
        return;
    }
    cout << "Executing " << *this << endl;

    origin->removeArmies(armies);
    origin->freeArmies(armies);

    if (dest->getPlayer() == player) {
        dest->addArmies(armies);
        setEffect("Airlift " + to_string(armies) + " armies from territory "
                  + origin->getName() + " to territory " + dest->getName());
    } else {
        attackTerritory(origin, armies, dest);
        setEffect("Airlift " + to_string(armies) + " armies from territory "
                  + origin->getName() + " to attack territory " + dest->getName());
    }

    setExecuted(true);
    cout << "Effect: " << getEffect() << endl;

}

/**
 * Helper print function for polymorphic stream insertion
 *
 * @param out
 */
void AirliftOrder::print(ostream &out) const {
    out << "AirliftOrder{ "
        << "executed: " << isExecuted();
    if (isExecuted()) {
        out << ", effect: " << getEffect();
    } else {
        out << ", armies: " << armies
            << ", origin: " << origin->getName()
            << ", dest: " << dest->getName();
    }
    out << " }";
}

/**
 * Helper function for polymorphic cloning
 */
AirliftOrder *AirliftOrder::clone() {
    return new AirliftOrder(*this);
}

//=============================
// NegotiateOrder Implementation
//=============================

NegotiateOrder::NegotiateOrder(Player *player)
        : Order(OrderType::NEGOTIATE),
          player{player} {}

/**
 * Validates current Order
 *
 * @param map
 * @param player
 * @return if order is valid or not
 */
bool NegotiateOrder::validate(Map *map, Player *player) {
    // Player can't negotiate with themselves
    if (this->player == player) {
        return false;
    }
    return true;
}

/**
 * Executes current order, modifying the game state if needed
 *
 * @param map Current map
 * @param player Player executing the order
 */
void NegotiateOrder::execute(Map *map, Player *player) {
    if (isExecuted()) {
        cout << *this << " was already executed. Not executing." << endl;
        return;
    }
    if (!validate(map, player)) {
        cout << *this << " is invalid. Not executing." << endl;
        return;
    }
    cout << "Executing " << *this << endl;

    player->addAlly(this->player);
    setEffect("Players " + this->player->getName() + " and " + player->getName()
              + " are now allies for one turn.");

    setExecuted(true);
    cout << "Effect: " << getEffect() << endl;

}

/**
 * Helper print function for polymorphic stream insertion
 *
 * @param out
 */
void NegotiateOrder::print(ostream &out) const {
    out << "NegotiateOrder{ "
        << "executed: " << isExecuted();
    if (isExecuted()) {
        out << ", effect: " << getEffect();
    } else {
        out << ", player: " << player->getName();
    }
    out << " }";
}

/**
 * Helper function for polymorphic cloning
 */
NegotiateOrder *NegotiateOrder::clone() {
    return new NegotiateOrder(*this);
}
