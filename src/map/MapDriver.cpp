#include <iostream>

#include "Map.h"

using std::cout;
using std::endl;
using std::boolalpha;

void evaluateMap(Map &map);

/**
 * Driver for Map
 */
int main() {
    cout << boolalpha;

    Map emptyMap("This is an empty map!");
    evaluateMap(emptyMap);

    // Invalid map: Map isn't a connected graph
    Map unconnectedMap = Map("This map has a territory which isn't connected!");

    unconnectedMap.addContinent("Continent 0", 1);
    unconnectedMap.addContinent("Continent 1", 1);
    unconnectedMap.addTerritory("t0", 0, 1);
    unconnectedMap.addTerritory("t1", 0, 1);
    unconnectedMap.addTerritory("t2", 0, 1);
    unconnectedMap.addTerritory("t3", 0, 1);
    unconnectedMap.addTerritory("t4", 0, 1);
    unconnectedMap.addTerritory("t5", 0, 1);
    unconnectedMap.addConnection(2, 1);
    unconnectedMap.addConnection(0, 2);
    unconnectedMap.addConnection(3, 0);
    unconnectedMap.addConnection(1, 3);
    unconnectedMap.addConnection(3, 4);

    evaluateMap(unconnectedMap);

    // Invalid map: One of the continents isn't a connected subgraph
    Map unconnectedContinent = Map("This map has a continent which isn't connected!");

    unconnectedContinent.addContinent("Continent 0", 1);
    unconnectedContinent.addContinent("Continent 1", 1);
    unconnectedContinent.addTerritory("t0", 0, 1);
    unconnectedContinent.addTerritory("t1", 0, 1);
    unconnectedContinent.addTerritory("t2", 1, 1);
    unconnectedContinent.addTerritory("t3", 1, 1);
    unconnectedContinent.addTerritory("t4", 1, 1);
    unconnectedContinent.addTerritory("t5", 1, 1);
    unconnectedContinent.addConnection(0, 2);
    unconnectedContinent.addConnection(1, 2);
    unconnectedContinent.addConnection(3, 2);
    unconnectedContinent.addConnection(4, 3);
    unconnectedContinent.addConnection(5, 4);

    evaluateMap(unconnectedContinent);

    // Invalid map: One of the territories has an invalid continent
    Map badContinent = Map("This map has a territories with an invalid continent!");
    Continent *outsideContinent = new Continent("outside continent", 1);
    badContinent.addContinent("Continent 0", 1);
    badContinent.addTerritory("t0", 0, 1);
    badContinent.addTerritory("t1", 0, 1);
    badContinent.addConnection(0, 1);
    badContinent.getTerritories()[0]->setContinent(outsideContinent);

    evaluateMap(badContinent);

    // Valid map
    Map validMap = Map("Valid Map");
    validMap.addContinent("Continent 0", 1);
    validMap.addContinent("Continent 1", 1);
    validMap.addTerritory("t0", 0, 1);
    validMap.addTerritory("t1", 0, 1);
    validMap.addTerritory("t2", 0, 1);
    validMap.addTerritory("t3", 0, 1);
    validMap.addTerritory("t4", 1, 1);
    validMap.addTerritory("t5", 1, 1);
    validMap.addConnection(2, 1);
    validMap.addConnection(0, 2);
    validMap.addConnection(3, 0);
    validMap.addConnection(1, 3);
    validMap.addConnection(3, 4);
    validMap.addConnection(4, 5);

    evaluateMap(validMap);

    delete outsideContinent;
}

void evaluateMap(Map &map) {
    cout << map;
    bool valid = map.validate();
    cout << "Valid: " << valid << endl << endl << endl;
}