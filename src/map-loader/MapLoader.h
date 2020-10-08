#pragma once

#include <string>
#include <iostream>

using std::ostream;
using std::string;

class Map;

class MapLoader {

public:
    MapLoader();
    MapLoader(const MapLoader &other);
    friend void swap(MapLoader &a, MapLoader &b);
    MapLoader &operator=(MapLoader other);
    friend ostream &operator<<(ostream &out, const MapLoader &obj);
    static Map *readMapFile(string fileName);

    ~MapLoader();
};
