#include <iostream>
#include "MapLoader.h"
#include "../map/Map.h"

using std::cout;
using std::endl;
using std::boolalpha;

void evaluateMap(Map &map);

/**
 * Driver for MapLoader
 */
int main() {
    cout << boolalpha;
    // Valid
    Map *zertina = MapLoader::readMapFile("zertina.map", "Zertina");
    evaluateMap(*zertina);

    // Valid
    Map *canada = MapLoader::readMapFile("canada.map", "Canada");
    evaluateMap(*canada);

    // Valid
    Map *usa = MapLoader::readMapFile("usa8regions.map", "USA");
    evaluateMap(*usa);

    // Invalid
    Map *invalid = MapLoader::readMapFile("invalid.map", "Invalid");
    evaluateMap(*invalid);

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