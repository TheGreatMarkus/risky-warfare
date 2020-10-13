#include "Orders.h"
#include "../player/Player.h"
#include "../map/Map.h"

using std::cout;
using std::endl;
using std::stringstream;
using std::to_string;
using std::boolalpha;

// TODO update validation to not hit allies

//=============================
// OrdersList Implementation
//=============================

OrdersList::OrdersList() : orders{vector<Order *>()} {}

OrdersList::OrdersList(const OrdersList &other) : orders{vector<Order *>()} {
    for (const auto &order : other.orders) {
        orders.push_back(order->clone());
    }
}

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

void OrdersList::add(Order *order) {
    orders.push_back(order);
}

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

OrdersList::~OrdersList() = default;


//=============================
// Order Implementation
//=============================

Order::Order() : executed{false} {}

Order::Order(const Order &other) : executed{other.executed} {}

void swap(Order &a, Order &b) {
    using std::swap;

    swap(a.executed, b.executed);
}

ostream &operator<<(ostream &out, const Order &obj) {
    obj.print(out);
    return out;
}

const bool &Order::getExecuted() const {
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


Order::~Order() = default;


//=============================
// DeployedOrder Implementation
//=============================

DeployOrder::DeployOrder(int armies, int territory)
        : armies{armies},
          territory{territory} {}

DeployOrder::DeployOrder(const DeployOrder &other)
        : Order(other),
          armies{other.armies},
          territory{other.territory} {}

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

bool DeployOrder::validate(Map *map, Player *player) {
    if (territory >= map->getTerritories().size()
        || player->owns(territory)) {
        return false;
    }
    return true;
}

void DeployOrder::execute(Map *map, Player *player) {
    if (validate(map, player)) {
        map->getTerritories()[territory]->addArmies(armies);
        setEffect("Added " + to_string(armies) + " armies to territory " + to_string(territory));
    }
}

ostream &DeployOrder::print(ostream &out) const {
    out << boolalpha << "DeployOrder{ "
        << "executed: " << getExecuted()
        << ", effect: \"" << getEffect()
        << "\", armies: " << armies
        << ", territory: " << territory
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

AdvanceOrder::AdvanceOrder(int armies, int originTerr, int destTerr)
        : armies{armies},
          origin{originTerr},
          dest{destTerr} {}

AdvanceOrder::AdvanceOrder(const AdvanceOrder &other)
        : Order(other),
          armies{other.armies},
          origin{other.origin},
          dest{other.dest} {}

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


bool AdvanceOrder::validate(Map *map, Player *player) {
    if (origin >= map->getTerritories().size()
        || dest >= map->getTerritories().size()) {
        return false;
    }

    Territory *originTerritory = map->getTerritories()[origin];
    Territory *destTerritory = map->getTerritories()[dest];

    if (!player->owns(origin) || !map->areAdjacent(origin, dest)
        || originTerritory->getArmies() < armies) {
        return false;
    }

    return true;
}

void AdvanceOrder::execute(Map *map, Player *player) {
    if (validate(map, player)) {
        map->getTerritories()[origin]->removeArmies(armies);
        map->getTerritories()[dest]->addArmies(armies);
        setEffect("Moved " + to_string(armies) + " armies from territory "
                  + to_string(origin) + " to territory " + to_string(dest));
    }
}

ostream &AdvanceOrder::print(ostream &out) const {
    out << boolalpha << "AdvanceOrder{ "
        << "executed: " << getExecuted()
        << ", effect: \"" << getEffect()
        << "\", armies: " << armies
        << ", origin: " << origin
        << ", dest: " << dest
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

BombOrder::BombOrder(int territory) : territory{territory} {}

BombOrder::BombOrder(const BombOrder &other)
        : Order(other),
          territory{other.territory} {}

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

bool BombOrder::validate(Map *map, Player *player) {
    if (territory >= map->getTerritories().size()) {
        return false;
    }

    if (player->owns(territory)) {
        return false;
    }
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

void BombOrder::execute(Map *map, Player *player) {
    if (validate(map, player)) {
        map->getTerritories()[territory]->bomb();
        setEffect("Halved armies for territory " + to_string(territory));
    }
}

ostream &BombOrder::print(ostream &out) const {
    out << boolalpha << "BombOrder{ "
        << "executed: " << getExecuted()
        << ", effect: \"" << getEffect()
        << "\", territory: " << territory
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

BlockadeOrder::BlockadeOrder(int territory) : territory{territory} {}

BlockadeOrder::BlockadeOrder(const BlockadeOrder &other)
        : Order(other),
          territory{other.territory} {}

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

bool BlockadeOrder::validate(Map *map, Player *player) {
    if (territory >= map->getTerritories().size()) {
        return false;
    }

    if (!player->owns(territory)) {
        return false;
    }

    return true;
}

void BlockadeOrder::execute(Map *map, Player *player) {
    if (validate(map, player)) {
        map->getTerritories()[territory]->blockade();
        player->removeTerritory(territory);
        setEffect("Blockaded on territory " + to_string(territory));
    }
}

ostream &BlockadeOrder::print(ostream &out) const {
    out << boolalpha << "BlockadeOrder{ "
        << "executed: " << getExecuted()
        << ", effect: \"" << getEffect()
        << "\", territory: " << territory
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

AirliftOrder::AirliftOrder(int armies, int origin, int dest)
        : armies{armies},
          origin{origin},
          dest{dest} {}

AirliftOrder::AirliftOrder(const AirliftOrder &other)
        : Order(other),
          armies{other.armies},
          origin{other.origin},
          dest{other.dest} {}

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

bool AirliftOrder::validate(Map *map, Player *player) {
    if (origin >= map->getTerritories().size()
        || dest >= map->getTerritories().size()) {
        return false;
    }

    if (!player->owns(origin) || !player->owns(dest)) {
        return false;
    }

    if (map->getTerritories()[origin]->getArmies() < armies) {
        return false;
    }

    return true;
}

void AirliftOrder::execute(Map *map, Player *player) {
    if (validate(map, player)) {
        map->getTerritories()[origin]->removeArmies(armies);
        map->getTerritories()[origin]->removeArmies(armies);
        setEffect("Airlift " + to_string(armies) + " armies from territory "
        + to_string(origin) + " to territory " + to_string(dest));
    }
}

ostream &AirliftOrder::print(ostream &out) const {
    out << boolalpha << "AirliftOrder{ "
        << "executed: " << getExecuted()
        << ", effect: \"" << getEffect()
        << "\", armies: " << armies
        << ", origin: " << origin
        << ", dest: " << dest
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

NegotiateOrder::NegotiateOrder(int playerId)
        : player{playerId} {}

NegotiateOrder::NegotiateOrder(const NegotiateOrder &other)
        : Order(other),
          player{other.player} {}

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


bool NegotiateOrder::validate(Map *map, Player *player) {
    if( this->player == player->getId()){
        return false;
    }
    return true;
}

void NegotiateOrder::execute(Map *map, Player *player) {
    if (validate(map, player)) {
        player->addAlly(this->player);
        setEffect("Players " + to_string(this->player) + " and " + to_string(player->getId())
                  + " are now allies for one turn.");
    }
}

ostream &NegotiateOrder::print(ostream &out) const {
    out << boolalpha << "NegotiateOrder{ "
        << "executed: " << getExecuted()
        << ", effect: \"" << getEffect()
        << "\", player: " << player
        << " }";
    return out;
}

NegotiateOrder *NegotiateOrder::clone() {
    return new NegotiateOrder(*this);
};

NegotiateOrder::~NegotiateOrder() {}





