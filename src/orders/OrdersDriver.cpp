#include "Orders.h"
#include "../map/Map.h"
#include "../player/Player.h"

using std::cout;
using std::endl;

/**
 * Driver for Orders.
 */
int main() {
    OrdersList list{};
    auto map = new Map{"Test Map"};
    auto player1 = new Player{0, "p0"};
    auto player2 = new Player{1, "p0"};

    player1->addTerritory(0);
    player2->addTerritory(1);

    map->addContinent("c0", 10);
    map->addTerritory("t0", 0, 10);
    map->addTerritory("t1", 0, 10);
    map->addConnection(0, 1);

    // You can play with these values to make them false
    auto deployOrder = new DeployOrder(2, 0);
    auto advanceOrder = new AdvanceOrder(2, 0, 1);
    auto bombOrder = new BombOrder(1);
    auto blockadeOrder = new BlockadeOrder(0);
    auto airliftOrder = new AirliftOrder(2, 0, 0);
    auto negotiateOrder = new NegotiateOrder(1);

    list.add(deployOrder);
    list.add(advanceOrder);
    list.add(bombOrder);
    list.add(blockadeOrder);
    list.add(airliftOrder);
    list.add(negotiateOrder);

    cout << *deployOrder << " - Valid?: " << deployOrder->validate(map, player1) << endl;
    cout << *advanceOrder << " - Valid: " << advanceOrder->validate(map, player1) << endl;
    cout << *bombOrder << " - Valid: " << bombOrder->validate(map, player1) << endl;
    cout << *blockadeOrder << " - Valid: " << blockadeOrder->validate(map, player1) << endl;
    cout << *airliftOrder << " - Valid: " << airliftOrder->validate(map, player1) << endl;
    cout << *negotiateOrder << " - Valid: " << negotiateOrder->validate(map, player1) << endl;

    deployOrder->execute(map, player1);
    advanceOrder->execute(map, player1);
    bombOrder->execute(map, player1);
    blockadeOrder->execute(map, player1);
    airliftOrder->execute(map, player1);
    negotiateOrder->execute(map, player1);

    list.move(0, 2);
    list.remove(0);

    cout << endl << list << endl;

    cout << endl << endl << *deployOrder << endl;
    cout << *advanceOrder << endl;
    cout << *bombOrder << endl;
    cout << *blockadeOrder << endl;
    cout << *airliftOrder << endl;
    cout << *negotiateOrder << endl;

    delete map;
    delete player1;
    delete player2;
}