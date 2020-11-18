#include "Orders.h"
#include "../map/Map.h"
#include "../player/Player.h"

using std::cout;
using std::endl;

/**
 * Driver for Orders.
 */
int main() {
    cout << std::boolalpha;

    OrdersList list{};
    Map *map = new Map{"Test Map"};
    Player *player1 = new Player{"p0"};
    Player *player2 = new Player{"p0"};

    map->addContinent("c0", 10);
    map->addTerritory("t0", 0, 10);
    map->addTerritory("t1", 0, 10);
    map->addTerritory("t2", 0, 10);
    map->addConnection(0, 1);
    map->addConnection(1, 2);

    Territory *t0 = map->getTerritories()[0];
    Territory *t1 = map->getTerritories()[1];
    Territory *t2 = map->getTerritories()[2];

    player1->captureTerritory(t0);
    player1->captureTerritory(t1);
    player2->captureTerritory(t2);

    // You can play with these values to make them false
    Order *deployOrder = new DeployOrder(2, t0);
    Order *advanceOrder = new AdvanceOrder(2, t0, t1);
    Order *bombOrder = new BombOrder(t2);
    Order *airliftOrder = new AirliftOrder(2, t0, t1);
    Order *negotiateOrder = new NegotiateOrder(player2);
    Order *blockadeOrder = new BlockadeOrder(t0);

    list.add(deployOrder);
    list.add(advanceOrder);
    list.add(bombOrder);
    list.add(airliftOrder);
    list.add(negotiateOrder);
    list.add(blockadeOrder);

    cout << *deployOrder << " - Valid?: " << deployOrder->validate(map, player1) << endl;
    cout << *advanceOrder << " - Valid: " << advanceOrder->validate(map, player1) << endl;
    cout << *bombOrder << " - Valid: " << bombOrder->validate(map, player1) << endl;
    cout << *airliftOrder << " - Valid: " << airliftOrder->validate(map, player1) << endl;
    cout << *negotiateOrder << " - Valid: " << negotiateOrder->validate(map, player1) << endl;
    cout << *blockadeOrder << " - Valid: " << blockadeOrder->validate(map, player1) << endl;

    deployOrder->execute(map, player1);
    advanceOrder->execute(map, player1);
    bombOrder->execute(map, player1);
    airliftOrder->execute(map, player1);
    negotiateOrder->execute(map, player1);
    blockadeOrder->execute(map, player1);

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