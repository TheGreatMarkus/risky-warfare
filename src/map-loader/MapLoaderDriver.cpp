#include <iostream>
#include "MapLoader.h"
#include "../map/Map.h"

using std::cout;
using std::endl;

void evaluateMap(Map &map);

/**
 * Driver for MapLoader
 */
int main() {
    cout << std::boolalpha;
    MapLoader *mapLoader = new MapLoader();

    // Valid
    Map *zertina = mapLoader->readMapFile("zertina.map", "Zertina");
    evaluateMap(*zertina);

    // Valid
    Map *canada = mapLoader->readMapFile("canada.map", "Canada");
    evaluateMap(*canada);

    // Valid
    Map *usa = mapLoader->readMapFile("usa8regions.map", "USA");
    evaluateMap(*usa);

    // Invalid
    Map *invalid = mapLoader->readMapFile("invalid.map", "Invalid");
    evaluateMap(*invalid);

    delete mapLoader;
    delete zertina;
    delete canada;
    delete usa;
    delete invalid;
}

void evaluateMap(Map &map) {
    cout << map;
    bool valid = map.validate();
    cout << "Valid: " << valid << endl << endl << endl;
}
