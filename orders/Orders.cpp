#include "Orders.h"
#include <iostream>

OrdersList::OrdersList(/* args */)
{
}

bool OrdersList::moveOrder(int origin, int dest)
{
    Order *tempOrder = orders->at(origin);

    try
    {
        orders->erase(orders->begin() + origin);
        orders->insert(orders->begin() + dest, tempOrder);
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
        return false;
    }

    return true;
}

bool OrdersList::deleteOrder(int i)
{
    try
    {
        orders->erase(orders->begin() + i);
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
        return false;
    }
    return true;
}

OrdersList::~OrdersList()
{
}

Order::Order(/* args */)
{
}

Order::~Order()
{
}

DeployOrder::DeployOrder(/* args */)
{
}

DeployOrder::~DeployOrder()
{
}

AdvanceOrder::AdvanceOrder(/* args */)
{
}

AdvanceOrder::~AdvanceOrder()
{
}

BombOrder::BombOrder(/* args */)
{
}

BombOrder::~BombOrder()
{
}

BlockadeOrder::BlockadeOrder(/* args */)
{
}

BlockadeOrder::~BlockadeOrder()
{
}

AirliftOrder::AirliftOrder(/* args */)
{
}

AirliftOrder::~AirliftOrder()
{
}

NegotiateOrder::NegotiateOrder(/* args */)
{
}

NegotiateOrder::~NegotiateOrder()
{
}