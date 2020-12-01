#include "MapLoader.h"

#include <map>
#include <fstream>
#include <vector>

#include "../map/Map.h"
#include "../utils/Utils.h"

using std::map;
using std::string;
using std::cout;
using std::endl;
using std::ifstream;

using cris_utils::strSplit;
using cris_utils::trim;
using cris_utils::isNumber;

namespace {
    /**
     * Prints an error message, and the line in the .map file where the error occurred
     *
     * @param message
     * @param lineNum
     */
    void printError(string message, int lineNum) {
        cout << "ERROR: \"" << message << "\" IN MAP FILE AT LINE " << lineNum << endl;
    }

    /**
     * Describes the section of the .map file currently being read
     */
    enum class Section {
        continents,
        territories,
        borders,
        none,
    };
}

//=============================
// MapLoader Implementation
//=============================

ostream &operator<<(ostream &out, const MapLoader &obj) {
    out << "MapLoader";
    return out;
}

/**
 * Reads a given WarZone .map file and return the equivalent Map object
 *
 * @param path The path to the .map file
 * @param name Name of the map
 * @return a Map pointer
 */
Map *MapLoader::readMapFile(string path, string name) {
    Section currentSection(Section::none);
    ifstream mapFile(path);
    string line;

    Map *map = new Map(name);
    int lineNum = 0;

    while (getline(mapFile, line)) {
        lineNum++;
        // Clean line to avoid issues with special characters (line carriage, etc.)
        trim(line);

        // Skip empty or comment lines
        if (line.empty() || line[0] == ';') {
            continue;
        }

        // If current line is a header for a section, set currentSection accordingly
        if (line == "[files]") {
            currentSection = Section::none;
            continue;
        } else if (line == "[continents]") {
            currentSection = Section::continents;
            continue;
        } else if (line == "[countries]") {
            currentSection = Section::territories;
            continue;
        } else if (line == "[borders]") {
            currentSection = Section::borders;
            continue;
        }

        switch (currentSection) {
            case Section::continents: {
                vector<string> tokens = strSplit(line, " ");
                // Expected format of line: "ContinentName ArmyValue"
                if (tokens.size() < 2 || !isNumber(tokens[1])) {
                    printError("INVALID CONTINENT", lineNum);
                    return map;
                }
                map->addContinent(tokens[0], stoi(tokens[1]));
                break;
            }

            case Section::territories: {
                vector<string> tokens = strSplit(line, " ");
                // Expected format of line: "TerritoryId TerritoryName ContinentId"
                if (tokens.size() < 3 || !isNumber(tokens[0]) || !isNumber(tokens[2])) {
                    printError("INVALID COUNTRY/TERRITORY", lineNum);
                    return map;
                }
                int continentIndex = stoi(tokens[2]) - 1;
                map->addTerritory(tokens[1], continentIndex, 0);
                break;
            }
            case Section::borders: {
                vector<string> tokens = strSplit(line, " ");
                // Expected format of line: "TerritoryId Neighbor1 Neighbor2 ..."
                if (tokens.size() < 2) {
                    printError("INVALID BORDERS", lineNum);
                    return map;
                }
                int terr1Id = stoi(tokens[0]);
                for (int i = 1; i < tokens.size(); i++) {
                    if (!isNumber(tokens[i])) {
                        printError("INVALID BORDERS", lineNum);
                        return map;
                    }
                    int terr2Id = stoi(tokens[i]);
                    map->addConnection(terr1Id - 1, terr2Id - 1);
                }
                break;
            }
            case Section::none:
                break;
        }
    }
    return map;
}

MapLoader::~MapLoader() {}

ostream &operator<<(ostream &out, const ConquestFileReader &obj) {
    out << "ConquestFileReader";
    return out;
}

/**
 * Reads a given Conquest .map file and return the equivalent Map object
 *
 * @param path The path to the .map file
 * @param name Name of the map
 * @return a Map pointer
 */
Map *ConquestFileReader::readConquestFile(string path, string name) {
    Section currentSection(Section::none);
    ifstream mapFile(path);
    string line;

    Map *newMap = new Map(name);
    int lineNum = 0;
    map<string, int> territories;
    map<string, int> continents;
    map<string, vector<string>> connections;

    while (getline(mapFile, line)) {
        lineNum++;
        // Clean line to avoid issues with special characters (line carriage, etc.)
        trim(line);

        // Skip empty or comment lines
        if (line.empty() || line[0] == ';') {
            continue;
        }

        // If current line is a header for a section, set currentSection accordingly
        if (line == "[Map]") {
            currentSection = Section::none;
            continue;
        } else if (line == "[Continents]") {
            currentSection = Section::continents;
            continue;
        } else if (line == "[Territories]") {
            currentSection = Section::territories;
            continue;
        }

        switch (currentSection) {
            case Section::continents: {
                vector<string> tokens = strSplit(line, "=");
                // Expected format of line: "ContinentName=ArmyValue"
                if (tokens.size() < 2 || !isNumber(tokens[1])) {
                    printError("INVALID CONTINENT", lineNum);
                    return newMap;
                }
                continents[tokens[0]] = continents.size();
                newMap->addContinent(tokens[0], stoi(tokens[1]));
                break;
            }

            case Section::territories: {
                vector<string> tokens = strSplit(line, ",");
                // Expected format of line: "TerritoryName,x,y,ContinentName,neighbor1,neighbor2,..."
                if (tokens.size() < 5) {
                    printError("INVALID COUNTRY/TERRITORY", lineNum);
                    return newMap;
                }
                territories[tokens[0]] = territories.size();
                for (int i = 4; i < tokens.size(); ++i) {
                    connections[tokens[0]].push_back(tokens[i]);
                }
                newMap->addTerritory(tokens[0], continents[tokens[3]], 0);
                break;
            }
            case Section::borders:
            case Section::none:
                break;
        }
    }
    // Build connections after getting all territories and their indices
    for (auto &pair : connections) {
        for (auto &neighbor : pair.second) {
            newMap->addConnection(territories[pair.first], territories[neighbor]);
        }
    }
    return newMap;
}

ConquestFileReaderAdapter::ConquestFileReaderAdapter() : conquestFileReader{new ConquestFileReader()} {}

ostream &operator<<(ostream &out, const ConquestFileReaderAdapter &obj) {
    out << "ConquestFileReaderAdapter";
    return out;
}

/**
 * Reads a given Conquest .map file using the MapLoader interface
 *
 * Delegates map loading to ConquestFileReader
 *
 * @param path The path to the .map file
 * @param name Name of the map
 * @return a Map pointer
 */
Map *ConquestFileReaderAdapter::readMapFile(string path, string name) {
    return conquestFileReader->readConquestFile(path, name);
}

ConquestFileReaderAdapter::~ConquestFileReaderAdapter() {
    delete conquestFileReader;
}


