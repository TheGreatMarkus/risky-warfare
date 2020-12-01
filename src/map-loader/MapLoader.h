#pragma once

#include <string>
#include <iostream>

using std::ostream;
using std::string;

class Map;

/**
 * Class for loading WarZone map files and transforming them to Map objects.
 */
class MapLoader {
public:
    friend ostream &operator<<(ostream &out, const MapLoader &obj);

    virtual Map *readMapFile(string path, string name);

    virtual ~MapLoader();
};

/**
 * Class for loading Conquest map files and transforming them to Map objects.
 */
class ConquestFileReader {

public:
    friend ostream &operator<<(ostream &out, const ConquestFileReader &obj);

    Map *readConquestFile(string path, string name);
};

/**
 * Adapter for loading Conquest map files with a MapLoader interface
 */
class ConquestFileReaderAdapter : public MapLoader {
private:
    ConquestFileReader *conquestFileReader;
public:
    ConquestFileReaderAdapter();
    friend ostream &operator<<(ostream &out, const ConquestFileReaderAdapter &obj);

    Map *readMapFile(string path, string name);

    ~ConquestFileReaderAdapter();
};
