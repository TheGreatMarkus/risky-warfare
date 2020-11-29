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

    virtual Map *readMapFile(string path, string name);

    ~MapLoader();
};

class ConquestFileReader {

public:
    friend ostream &operator<<(ostream &out, const ConquestFileReader &obj);

    Map *readConquestFile(string path, string name);
};

class ConquestFileReaderAdapter : public MapLoader {
private:
    ConquestFileReader *conquestFileReader;
public:
    ConquestFileReaderAdapter();
    ConquestFileReaderAdapter(const ConquestFileReaderAdapter &other);
    friend void swap(ConquestFileReaderAdapter &a, ConquestFileReaderAdapter &b);
    ConquestFileReaderAdapter &operator=(ConquestFileReaderAdapter other);
    friend ostream &operator<<(ostream &out, const ConquestFileReaderAdapter &obj);

    Map *readMapFile(string path, string name);

    ~ConquestFileReaderAdapter();
};
