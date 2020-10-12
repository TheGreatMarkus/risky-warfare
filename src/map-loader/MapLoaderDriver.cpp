#include <iostream>
#include "MapLoader.h"
#include "../map/Map.h"

using std::cout;
using std::endl;
using std::ofstream;

void evaluateMap(Map &map);

int main() {
    // Valid
    Map *zertina = MapLoader::readMapFile("zertina.map");
    evaluateMap(*zertina);

    // Valid
    Map *canada = MapLoader::readMapFile("canada.map");
    evaluateMap(*canada);

    // Valid
    Map *usa = MapLoader::readMapFile("usa8regions.map");
    evaluateMap(*usa);
    return 0;
}

void evaluateMap(Map &map) {
    cout << map;
    bool valid = map.validate();
    cout << "Valid: " << valid << endl;
}