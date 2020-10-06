#pragma once

#include "../map/Map.h"
#include <string>

using namespace std;

class MapLoader
{
public:
    MapLoader(/* args */);
    static void readMapFile(string fileName);
    ~MapLoader();
};
