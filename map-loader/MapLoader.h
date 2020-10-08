#pragma once

#include "../map/Map.h"
#include <string>

class MapLoader
{

public:
    MapLoader();

    MapLoader &operator=(MapLoader other);
    friend void swap(MapLoader &first, MapLoader &second);
    friend ostream &operator<<(ostream &out, const MapLoader &obj);

    static MapLoader *readMapFile(string fileName);
    ~MapLoader();
};
