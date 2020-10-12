#include <iostream>

#include "Map.h"

using std::cout;
using std::endl;
using std::boolalpha;

void evaluateMap(Map &map);

int main() {
    cout << boolalpha;

    Map emptyMap("This is an empty map!");
    evaluateMap(emptyMap);

    // Invalid map: Map isn't a connected graph
    Map unconnectedMap = Map("This map has a territory which isn't connected!");

    unconnectedMap.addContinent("Continent 0", 1);
    unconnectedMap.addContinent("Continent 1", 1);
    unconnectedMap.addTerritory("c0-t0", 0, 1, 1);
    unconnectedMap.addTerritory("c0-t1", 0, 0, 1);
    unconnectedMap.addTerritory("c0-t2", 0, 0, 1);
    unconnectedMap.addTerritory("c0-t3", 0, 0, 1);
    unconnectedMap.addTerritory("c0-t4", 0, 0, 1);
    unconnectedMap.addTerritory("c0-t5", 0, 1, 1);
    unconnectedMap.addConnection(3, 2);
    unconnectedMap.addConnection(1, 3);
    unconnectedMap.addConnection(4, 1);
    unconnectedMap.addConnection(2, 4);
    unconnectedMap.addConnection(4, 5);

    evaluateMap(unconnectedMap);

    // Invalid map: One of the continents isn't a connected subgraph
    Map unconnectedContinent = Map("This map has a territory which isn't connected!");

    unconnectedContinent.addContinent("Continent 0", 1);
    unconnectedContinent.addContinent("Continent 1", 1);
    unconnectedContinent.addTerritory("c0-t0", 0, 0, 1);
    unconnectedContinent.addTerritory("c0-t1", 0, 0, 1);
    unconnectedContinent.addTerritory("c1-t0", 1, 0, 1);
    unconnectedContinent.addTerritory("c1-t2", 1, 0, 1);
    unconnectedContinent.addTerritory("c1-t3", 1, 1, 1);
    unconnectedContinent.addTerritory("c1-t4", 1, 1, 1);
    unconnectedContinent.addConnection(1, 3);
    unconnectedContinent.addConnection(2, 3);
    unconnectedContinent.addConnection(4, 3);
    unconnectedContinent.addConnection(5, 4);
    unconnectedContinent.addConnection(6, 5);

    evaluateMap(unconnectedContinent);

    // TODO: Invalid map: Continent missing for one territory.

    // Valid map
    Map validMap = Map("Valid Map");
    validMap.addContinent("Continent 0", 1);
    validMap.addContinent("Continent 1", 1);
    validMap.addTerritory("c0-t1", 0, 0, 1);
    validMap.addTerritory("c0-t2", 0, 0, 1);
    validMap.addTerritory("c0-t3", 0, 0, 1);
    validMap.addTerritory("c0-t4", 0, 0, 1);
    validMap.addTerritory("c1-t1", 1, 1, 1);
    validMap.addTerritory("c1-t2", 1, 1, 1);
    validMap.addConnection(3, 2);
    validMap.addConnection(1, 3);
    validMap.addConnection(4, 1);
    validMap.addConnection(2, 4);
    validMap.addConnection(4, 5);
    validMap.addConnection(5, 6);

    evaluateMap(validMap);

    return 0;
}

void evaluateMap(Map &map) {
    cout << map;
    bool valid = map.validate();
    cout << "Valid: " << valid << endl;
}