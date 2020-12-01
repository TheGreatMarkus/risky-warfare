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
    friend ostream &operator<<(ostream &out, const MapLoader &obj);

    virtual Map *readMapFile(string path, string name);

    virtual ~MapLoader();
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
    friend ostream &operator<<(ostream &out, const ConquestFileReaderAdapter &obj);

    Map *readMapFile(string path, string name);

    ~ConquestFileReaderAdapter();
};
