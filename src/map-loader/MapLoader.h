#pragma once

#include <string>
#include <iostream>

using std::ostream;
using std::string;

class Map;

/**
 * Class for loading .map files and transforming them to Map objects.
 */
class MapLoader {

public:
    MapLoader();
    MapLoader(const MapLoader &other);
    friend void swap(MapLoader &a, MapLoader &b);
    MapLoader &operator=(MapLoader other);
    friend ostream &operator<<(ostream &out, const MapLoader &obj);
    static Map *readMapFile(string path, string name);

    ~MapLoader();
};
