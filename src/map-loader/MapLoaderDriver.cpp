#include <iostream>
#include "MapLoader.h"
#include "../map/Map.h"

using std::cout;
using std::endl;
using std::ofstream;

int main() {
    Map *testMap = MapLoader::readMapFile("example.map");

    cout << (*testMap) << endl;
    return 0;
}