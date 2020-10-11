#include <iostream>

#include "Orders.h"
#include "../player/Player.h"
#include "../map/Map.h"

using std::cout;
using std::endl;

//=============================
// OrdersList Implementation
//=============================

OrdersList::OrdersList() : orders{vector<Order *>()} {}

OrdersList::OrdersList(const OrdersList &other) : orders{vector<Order *>()} {
    // TODO: Implement. Currently not sure since Order is an abstract class
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
    out << "OrdersList{" << endl;
    out << "orders: [" << endl;
    for (auto order : obj.orders) {
        out << *order << endl;
    }
    out << "]";
    return out;
}


bool OrdersList::moveOrder(int origin, int dest) {
    Order *tempOrder = orders.at(origin);

    try {
        orders.erase(orders.begin() + origin);
        orders.insert(orders.begin() + dest, tempOrder);
    }
    catch (const std::exception &e) {
        std::cerr << e.what() << '\n';
        return false;
    }

    return true;
}

bool OrdersList::deleteOrder(int i) {
    if (orders.size() <= i) {
        cout << "That index doesn't exist in the list!" << endl;
        return false;
    }

    orders.erase(orders.begin() + i);

    return true;
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

ostream &operator<<(ostream &out, Order &obj) {
    out << obj.toString();
    return out;
}

bool Order::getExecuted() const {
    return executed;
}

void Order::setExecuted(bool executed_) {
    executed = executed_;
}


Order::~Order() = default;

//=============================
// DeployedOrder Implementation
//=============================

DeployOrder::DeployOrder(int armies, int territoryId)
        : armies{armies},
          territoryId{territoryId} {}

DeployOrder::DeployOrder(const DeployOrder &other)
        : Order(other),
          armies{other.armies},
          territoryId{other.territoryId} {}

void swap(DeployOrder &a, DeployOrder &b) {
    using std::swap;

    swap(static_cast<Order &>(a), static_cast<Order &>(b));
    swap(a.armies, b.armies);
    swap(a.territoryId, b.territoryId);

}


DeployOrder &DeployOrder::operator=(DeployOrder other) {
    swap(*this, other);
    return *this;
}

bool DeployOrder::validate(Map *map, Player *player) {
    // TODO
    return false;
}

void DeployOrder::execute(Map *map, Player *player) {
    // TODO: Not implemented
}

string DeployOrder::toString() {
    return "DeployOrder{ executed: , armies: , originId: , destId: }";
}

DeployOrder::~DeployOrder() = default;

//=============================
// AdvanceOrder Implementation
//=============================

AdvanceOrder::AdvanceOrder(int armies, int originId, int destId)
        : armies{armies},
          originId{originId},
          destId{destId} {}

AdvanceOrder::AdvanceOrder(const AdvanceOrder &other)
        : Order(other),
          armies{other.armies},
          originId{other.originId},
          destId{other.destId} {}

void swap(AdvanceOrder &a, AdvanceOrder &b) {
    using std::swap;

    swap(static_cast<Order &>(a), static_cast<Order &>(b));
    swap(a.armies, b.armies);
    swap(a.originId, b.originId);
    swap(a.destId, b.destId);
}

AdvanceOrder &AdvanceOrder::operator=(AdvanceOrder other) {
    swap(*this, other);
    return *this;
}

bool AdvanceOrder::validate(Map *map, Player *player) {
    // TODO
    return false;
}

void AdvanceOrder::execute(Map *map, Player *player) {
    // TODO: Not implemented
}

string AdvanceOrder::toString() {
    return "TODO";
}

AdvanceOrder::~AdvanceOrder() = default;

//=============================
// BombOrder Implementation
//=============================

BombOrder::BombOrder(int territoryId) : territoryId{territoryId} {}

BombOrder::BombOrder(const BombOrder &other)
        : Order(other),
          territoryId{other.territoryId} {}

void swap(BombOrder &a, BombOrder &b) {
    using std::swap;

    swap(static_cast<Order &>(a), static_cast<Order &>(b));
    swap(a.territoryId, b.territoryId);
}

BombOrder &BombOrder::operator=(BombOrder other) {
    swap(*this, other);
    return *this;
}

bool BombOrder::validate(Map *map, Player *player) {
    // TODO
    return false;
}

void BombOrder::execute(Map *map, Player *player) {
    // TODO: Not implemented
}

string BombOrder::toString() {
    return "TODO";
}

BombOrder::~BombOrder() = default;


//=============================
// BlockadeOrder Implementation
//=============================

BlockadeOrder::BlockadeOrder(int territoryId) : territoryId{territoryId} {}

BlockadeOrder::BlockadeOrder(const BlockadeOrder &other)
        : Order(other),
          territoryId{territoryId} {}

void swap(BlockadeOrder &a, BlockadeOrder &b) {
    using std::swap;

    swap(static_cast<Order &>(a), static_cast<Order &>(b));
    swap(a.territoryId, b.territoryId);
}

BlockadeOrder &BlockadeOrder::operator=(BlockadeOrder other) {
    swap(*this, other);
    return *this;
}

bool BlockadeOrder::validate(Map *map, Player *player) {
    // TODO
    return false;
}

void BlockadeOrder::execute(Map *map, Player *player) {
    // TODO: Not implemented
}

string BlockadeOrder::toString() {
    return "TODO";
}

BlockadeOrder::~BlockadeOrder() = default;

//=============================
// AirliftOrder Implementation
//=============================

AirliftOrder::AirliftOrder(int armies, int originId, int destId)
        : armies{armies},
          originId{originId},
          destId{destId} {}

AirliftOrder::AirliftOrder(const AirliftOrder &other)
        : Order(other),
          armies{other.armies},
          originId{other.originId},
          destId{other.destId} {}

void swap(AirliftOrder &a, AirliftOrder &b) {
    using std::swap;

    swap(static_cast<Order &>(a), static_cast<Order &>(b));
    swap(a.armies, b.armies);
    swap(a.originId, b.originId);
    swap(a.destId, b.destId);
}

AirliftOrder &AirliftOrder::operator=(AirliftOrder other) {
    swap(*this, other);
    return *this;
}

bool AirliftOrder::validate(Map *map, Player *player) {
    // TODO
    return false;
}

void AirliftOrder::execute(Map *map, Player *player) {
    // TODO: Not implemented
}

string AirliftOrder::toString() {
    // TODO
    return "";
}

AirliftOrder::~AirliftOrder() = default;

//=============================
// NegotiateOrder Implementation
//=============================

NegotiateOrder::NegotiateOrder(int playerId)
        : playerId{playerId} {}

NegotiateOrder::NegotiateOrder(const NegotiateOrder &other)
        : Order(other),
          playerId{other.playerId} {}

void swap(NegotiateOrder &a, NegotiateOrder &b) {
    using std::swap;

    swap(static_cast<Order &>(a), static_cast<Order &>(b));
    swap(a.playerId, b.playerId);
}

NegotiateOrder &NegotiateOrder::operator=(NegotiateOrder other) {
    swap(*this, other);
    return *this;
}

bool NegotiateOrder::validate(Map *map, Player *player) {
    // TODO
    return false;
}

void NegotiateOrder::execute(Map *map, Player *player) {
    // TODO: Not implemented
}

string NegotiateOrder::toString() {
    // TODO
    return "";

}

NegotiateOrder::~NegotiateOrder() {};

