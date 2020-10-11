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

    Player player1(1, "player1");
    Player player2(2, "player2");

    newMap.addContinent("cont1", 1);
    newMap.addContinent("cont2", 1);

    newMap.addTerritory("t1", 0, 0, 1);
    newMap.addTerritory("t2", 0, 0, 1);
    newMap.addTerritory("t3", 0, 1, 1);
    newMap.addTerritory("t4", 0, 1, 1);
    newMap.addTerritory("t5", 0, 1, 1);

    vector<Territory *> territories(newMap.getTerritories());

    newMap.addConnection(1, 2);
    newMap.addConnection(2, 3);
    newMap.addConnection(3, 4);
    newMap.addConnection(4, 5);
    newMap.addConnection(5, 1);

    cout << newMap;

    cout << "valid: " << newMap.validate() << endl;

    return 0;
}