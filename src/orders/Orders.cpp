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

    void attackTerritory(Territory *attacker, int attackingArmies, Territory *defender) {
        int attackerKills = 0;
        int defenderKills = 0;
        for (int i = 0; i < attackingArmies; ++i) {
            int roll = randInt(1, 100);
            if (roll < ATTACK_CHANCE) {
                attackerKills++;
            }
        }

        for (int i = 0; i < defender->getArmies(); ++i) {
            int roll = randInt(1, 100);
            if (roll < DEFEND_CHANCE) {
                defenderKills++;
            }
        }

        defender->removeArmies(attackerKills);
        attackingArmies -= defenderKills;
        if (attackingArmies < 0) {
            attackingArmies = 0;
        }

        if (defender->getArmies() == 0 && attackingArmies > 0) {
            // Attack successful
            attacker->getPlayer()->captureTerritory(defender);
        } else if (defender->getArmies() > 0 && attackingArmies > 0) {
            // Attack failed, attacker has some armies left alive
            attacker->addArmies(attackingArmies);
        } else {
            // Attack failed, attacker has no armies left alive, nothing additional happens
        }
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
 * Swap method for copy-and-swap
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
    out << "OrdersList{" << endl
        << "orders: [" << endl;
    for (auto order : obj.orders) {
        out << *order << endl;
    }
    out << "]";
    return out;
}

Order *OrdersList::operator[](int i) {
    return orders[i];
}

const int OrdersList::size() const {
    return orders.size();
}

void OrdersList::add(Order *order) {
    orders.push_back(order);
}

/**
 * Moves order from origin to dest.
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

void OrdersList::remove(int i) {
    if (orders.size() <= i) {
        cout << "That index doesn't exist in the list!" << endl;
    }

    orders.erase(orders.begin() + i);
}


void OrdersList::remove(Order *order) {
    removeElement(orders, order);
}

Order *OrdersList::getHighestPriorityOrder() {
    Order *returnedOrder = nullptr;
    vector<OrderType> priorities = {OrderType::DEPLOY, OrderType::AIRLIFT, OrderType::BLOCKADE};

    for (auto &priority : priorities) {
        for (const auto &order : orders) {
            if (order->getType() == priority) {
                returnedOrder = order;
                break;
            }
        }
        if (returnedOrder != nullptr) {
            break;
        }
    }

    if (returnedOrder == nullptr && !orders.empty()) {
        returnedOrder = orders[0];
    }
    return returnedOrder;
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

Order::Order(const Order &other)
        : executed{other.executed},
          effect{other.effect},
          type{other.type} {}

/**
 * Swap method for copy-and-swap
 * @param a first element
 * @param b second element
 */
void swap(Order &a, Order &b) {
    using std::swap;

    swap(a.executed, b.executed);
}

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

void Order::setExecuted(bool executed_) {
    executed = executed_;
}

void Order::setEffect(string effect) {
    this->effect = effect;
}

OrderType Order::getType() const {
    return type;
}


Order::~Order() = default;


//=============================
// DeployedOrder Implementation
//=============================

DeployOrder::DeployOrder(int armies, Territory *territory)
        : Order(OrderType::DEPLOY),
          armies{armies},
          territory{territory} {}

DeployOrder::DeployOrder(const DeployOrder &other)
        : Order(other),
          armies{other.armies},
          territory{other.territory} {}

/**
* Swap method for copy-and-swap
* @param a first element
* @param b second element
*/
void swap(DeployOrder &a, DeployOrder &b) {
    using std::swap;

    swap(static_cast<Order &>(a), static_cast<Order &>(b));
    swap(a.armies, b.armies);
    swap(a.territory, b.territory);
}


DeployOrder &DeployOrder::operator=(DeployOrder other) {
    swap(*this, other);
    return *this;
}

ostream &operator<<(ostream &out, const DeployOrder &obj) {
    obj.print(out);
    return out;
}

/**
 * Validates current Order
 * @param map
 * @param player
 * @return Whether order is valid
 */
bool DeployOrder::validate(Map *map, Player *player) {
    // Territory must exist
    // Territory must be owned by the player
    // Player must have enough armies
    if (!contains(map->getTerritories(), territory)
        || !player->owns(territory)
        || player->getArmies() < armies) {
        return false;
    }
    return true;
}

/**
 * Executes current order, modifying the game state if needed.
 * @param map Current map
 * @param player Player executing the order
 */
void DeployOrder::execute(Map *map, Player *player) {
    if (validate(map, player)) {
        territory->addArmies(armies);
        setEffect("Added " + to_string(armies) + " armies to territory " + territory->getName());
        setExecuted(true);
    }
}

/**
 * Helper print function for polymorphic stream insertion.
 * @param out
 * @return out
 */
ostream &DeployOrder::print(ostream &out) const {
    out << "DeployOrder{ "
        << "executed: " << isExecuted()
        << ", effect: \"" << getEffect()
        << "\", armies: " << armies
        << ", territory: " << territory->getName()
        << " }";
    return out;
}

DeployOrder *DeployOrder::clone() {
    return new DeployOrder(*this);
}


DeployOrder::~DeployOrder() = default;


//=============================
// AdvanceOrder Implementation
//=============================

AdvanceOrder::AdvanceOrder(int armies, Territory *originTerr, Territory *destTerr)
        : Order(OrderType::ADVANCE),
          armies{armies},
          origin{originTerr},
          dest{destTerr} {}

AdvanceOrder::AdvanceOrder(const AdvanceOrder &other)
        : Order(other),
          armies{other.armies},
          origin{other.origin},
          dest{other.dest} {}

/**
* Swap method for copy-and-swap
* @param a first element
* @param b second element
*/
void swap(AdvanceOrder &a, AdvanceOrder &b) {
    using std::swap;

    swap(static_cast<Order &>(a), static_cast<Order &>(b));
    swap(a.armies, b.armies);
    swap(a.origin, b.origin);
    swap(a.dest, b.dest);
}

AdvanceOrder &AdvanceOrder::operator=(AdvanceOrder other) {
    swap(*this, other);
    return *this;
}

ostream &operator<<(ostream &out, const AdvanceOrder &obj) {
    obj.print(out);
    return out;
}

/**
 * Validates current Order
 * @param map
 * @param player
 * @return Whether order is valid
 */
bool AdvanceOrder::validate(Map *map, Player *player) {
    // both territories must exist
    // Player must own origin territory
    // origin and dest territory must be adjacent
    // origin must have enough troops
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
 * Executes current order, modifying the game state if needed.
 * @param map Current map
 * @param player Player executing the order
 */
void AdvanceOrder::execute(Map *map, Player *player) {
    if (validate(map, player)) {
        origin->removeArmies(armies);
        if (dest->getPlayer() == player) {
            dest->addArmies(armies);
            setEffect("Moved " + to_string(armies) + " armies from territory "
                      + origin->getName() + " to territory " + dest->getName());
        } else {
            attackTerritory(origin, armies, dest);
            setEffect("Moved " + to_string(armies) + " armies from territory "
                      + origin->getName() + " to attack territory " + dest->getName());
        }
        setExecuted(true);
    }
}

/**
 * Helper print function for polymorphic stream insertion.
 * @param out
 * @return out
 */
ostream &AdvanceOrder::print(ostream &out) const {
    out << "AdvanceOrder{ "
        << "executed: " << isExecuted()
        << ", effect: \"" << getEffect()
        << "\", armies: " << armies
        << ", origin: " << origin->getName()
        << ", dest: " << dest->getName()
        << " }";
    return out;
}

AdvanceOrder *AdvanceOrder::clone() {
    return new AdvanceOrder(*this);
}


AdvanceOrder::~AdvanceOrder() = default;


//=============================
// BombOrder Implementation
//=============================

BombOrder::BombOrder(Territory *territory) : Order(OrderType::BOMB), territory{territory} {}

BombOrder::BombOrder(const BombOrder &other)
        : Order(other),
          territory{other.territory} {}

/**
* Swap method for copy-and-swap
* @param a first element
* @param b second element
*/
void swap(BombOrder &a, BombOrder &b) {
    using std::swap;

    swap(static_cast<Order &>(a), static_cast<Order &>(b));
    swap(a.territory, b.territory);
}

BombOrder &BombOrder::operator=(BombOrder other) {
    swap(*this, other);
    return *this;
}

ostream &operator<<(ostream &out, const BombOrder &obj) {
    obj.print(out);
    return out;
}

/**
 * Validates current Order
 * @param map
 * @param player
 * @return Whether order is valid
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
 * Executes current order, modifying the game state if needed.
 * @param map Current map
 * @param player Player executing the order
 */
void BombOrder::execute(Map *map, Player *player) {
    if (validate(map, player)) {
        territory->bomb();
        setEffect("Bombed territory " + territory->getName());
        setExecuted(true);
    }
}

/**
 * Helper print function for polymorphic stream insertion.
 * @param out
 * @return out
 */
ostream &BombOrder::print(ostream &out) const {
    out << "BombOrder{ "
        << "executed: " << isExecuted()
        << ", effect: \"" << getEffect()
        << "\", territory: " << territory->getName()
        << " }";
    return out;
}

BombOrder *BombOrder::clone() {
    return new BombOrder(*this);
}


BombOrder::~BombOrder() = default;


//=============================
// BlockadeOrder Implementation
//=============================

BlockadeOrder::BlockadeOrder(Territory *territory) : Order(OrderType::BLOCKADE), territory{territory} {}

BlockadeOrder::BlockadeOrder(const BlockadeOrder &other)
        : Order(other),
          territory{other.territory} {}

/**
* Swap method for copy-and-swap
* @param a first element
* @param b second element
*/
void swap(BlockadeOrder &a, BlockadeOrder &b) {
    using std::swap;

    swap(static_cast<Order &>(a), static_cast<Order &>(b));
    swap(a.territory, b.territory);
}

BlockadeOrder &BlockadeOrder::operator=(BlockadeOrder other) {
    swap(*this, other);
    return *this;
}

ostream &operator<<(ostream &out, const BlockadeOrder &obj) {
    obj.print(out);
    return out;
}

/**
 * Validates current Order
 * @param map
 * @param player
 * @return Whether order is valid
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
 * Executes current order, modifying the game state if needed.
 * @param map Current map
 * @param player Player executing the order
 */
void BlockadeOrder::execute(Map *map, Player *player) {
    if (validate(map, player)) {
        territory->blockade();
        player->loseTerritory(territory);
        setEffect("Blockaded territory " + territory->getName());
        setExecuted(true);
    }
}

/**
 * Helper print function for polymorphic stream insertion.
 * @param out
 * @return out
 */
ostream &BlockadeOrder::print(ostream &out) const {
    out << "BlockadeOrder{ "
        << "executed: " << isExecuted()
        << ", effect: \"" << getEffect()
        << "\", territory: " << territory->getName()
        << " }";
    return out;
}

BlockadeOrder *BlockadeOrder::clone() {
    return new BlockadeOrder(*this);
}


BlockadeOrder::~BlockadeOrder() = default;


//=============================
// AirliftOrder Implementation
//=============================

AirliftOrder::AirliftOrder(int armies, Territory *origin, Territory *dest)
        : Order(OrderType::AIRLIFT),
          armies{armies},
          origin{origin},
          dest{dest} {}

AirliftOrder::AirliftOrder(const AirliftOrder &other)
        : Order(other),
          armies{other.armies},
          origin{other.origin},
          dest{other.dest} {}

/**
* Swap method for copy-and-swap
* @param a first element
* @param b second element
*/
void swap(AirliftOrder &a, AirliftOrder &b) {
    using std::swap;

    swap(static_cast<Order &>(a), static_cast<Order &>(b));
    swap(a.armies, b.armies);
    swap(a.origin, b.origin);
    swap(a.dest, b.dest);
}

AirliftOrder &AirliftOrder::operator=(AirliftOrder other) {
    swap(*this, other);
    return *this;
}

ostream &operator<<(ostream &out, const AirliftOrder &obj) {
    obj.print(out);
    return out;
}

/**
 * Validates current Order
 * @param map
 * @param player
 * @return Whether order is valid
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
 * Executes current order, modifying the game state if needed.
 * @param map Current map
 * @param player Player executing the order
 */
void AirliftOrder::execute(Map *map, Player *player) {
    if (validate(map, player)) {
        origin->removeArmies(armies);
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
    }
}

/**
 * Helper print function for polymorphic stream insertion.
 * @param out
 * @return out
 */
ostream &AirliftOrder::print(ostream &out) const {
    out << "AirliftOrder{ "
        << "executed: " << isExecuted()
        << ", effect: \"" << getEffect()
        << "\", armies: " << armies
        << ", origin: " << origin->getName()
        << ", dest: " << dest->getName()
        << " }";
    return out;
}

AirliftOrder *AirliftOrder::clone() {
    return new AirliftOrder(*this);
}


AirliftOrder::~AirliftOrder() = default;

//=============================
// NegotiateOrder Implementation
//=============================

NegotiateOrder::NegotiateOrder(Player *player)
        : Order(OrderType::NEGOTIATE),
          player{player} {}

NegotiateOrder::NegotiateOrder(const NegotiateOrder &other)
        : Order(other),
          player{other.player} {}

/**
* Swap method for copy-and-swap
* @param a first element
* @param b second element
*/
void swap(NegotiateOrder &a, NegotiateOrder &b) {
    using std::swap;

    swap(static_cast<Order &>(a), static_cast<Order &>(b));
    swap(a.player, b.player);
}

NegotiateOrder &NegotiateOrder::operator=(NegotiateOrder other) {
    swap(*this, other);
    return *this;
}

ostream &operator<<(ostream &out, const NegotiateOrder &obj) {
    obj.print(out);
    return out;
}

/**
 * Validates current Order
 * @param map
 * @param player
 * @return Whether order is valid
 */
bool NegotiateOrder::validate(Map *map, Player *player) {
    // Player can't negotiate with themselves
    if (this->player == player) {
        return false;
    }
    return true;
}

/**
 * Executes current order, modifying the game state if needed.
 * @param map Current map
 * @param player Player executing the order
 */
void NegotiateOrder::execute(Map *map, Player *player) {
    if (validate(map, player)) {
        player->addAlly(this->player);
        setEffect("Players " + this->player->getName() + " and " + player->getName()
                  + " are now allies for one turn.");
        setExecuted(true);
    }
}

/**
 * Helper print function for polymorphic stream insertion.
 * @param out
 * @return out
 */
ostream &NegotiateOrder::print(ostream &out) const {
    out << "NegotiateOrder{ "
        << "executed: " << isExecuted()
        << ", effect: \"" << getEffect()
        << "\", player: " << player->getName()
        << " }";
    return out;
}

NegotiateOrder *NegotiateOrder::clone() {
    return new NegotiateOrder(*this);
};

NegotiateOrder::~NegotiateOrder() {}
