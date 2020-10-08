#include <iostream>
#include <vector>

#include "Map.h"
#include "../player/Player.h"

using std::cout;
using std::endl;

int main() {
    // TODO: Valid map

    // TODO: Invalid map: Map not graph

    // TODO: Invalid map: Continent not subgraph

    // TODO: Invalid map: Continent missing for one territory.
    Map newMap = Map("newMap");

    Player player1(0, "player1");
    Player player2(1, "player2");

    newMap.addContinent("cont1", 1);
    newMap.addContinent("cont2", 1);

    newMap.addTerritory(1, "t1", 0, 0, 1);
    newMap.addTerritory(2, "t2", 0, 0, 1);
    newMap.addTerritory(3, "t3", 0, 1, 1);
    newMap.addTerritory(4, "t4", 1, 1, 1);

    vector<Territory *> territories(newMap.getTerritories());

    newMap.addConnection(territories.at(0)->getId(), territories.at(1)->getId());
    newMap.addConnection(territories.at(0)->getId(), territories.at(2)->getId());
    newMap.addConnection(territories.at(0)->getId(), territories.at(3)->getId());
    newMap.addConnection(territories.at(2)->getId(), territories.at(3)->getId());

    cout << newMap;

    cout << "valid: " << newMap.validate() << endl;

    return 0;
}