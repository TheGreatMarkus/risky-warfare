#include <iostream>

#include "Orders.h"

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

Order::Order(bool executed) : executed{executed} {}

Order::Order(const Order &other) : executed{other.executed} {}

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



DeployOrder::DeployOrder(bool executed, int armies, int originId, int destId)
        : Order(executed),
          armies{armies},
          originId{originId},
          destId{destId} {}

DeployOrder::DeployOrder(const DeployOrder &other)
        : Order(other.getExecuted()),
          armies{other.armies},
          originId{other.originId},
          destId{other.destId} {}

void swap(DeployOrder &a, DeployOrder &b) {
    using std::swap;

    swap(static_cast<Order &>(a), static_cast<Order &>(b));
    swap(a.armies, b.armies);
    swap(a.originId, b.originId);
    swap(a.destId, b.destId);

}

DeployOrder &DeployOrder::operator=(DeployOrder other) {
    swap(*this, other);
    return *this;
}

bool DeployOrder::validate() {
    // TODO
}

void DeployOrder::execute() {
    // TODO: Currently does nothing
}

string DeployOrder::toString() {
    return "DeployOrder{ executed: , armies: , originId: , destId: }";
}

DeployOrder::~DeployOrder() = default;
