#include "MapLoader.h"

#include <fstream>
#include <iostream>
#include <vector>
#include <string>

#include "../map/Map.h"
#include "../utils/Utils.h"

using std::cout;
using std::endl;
using std::ifstream;

using cris_utils::strSplit;
using cris_utils::trim;
using cris_utils::isNumber;

namespace {
    void printError(string message, int lineNum) {
        cout << "ERROR: \"" << message << "\" IN MAP FILE AT LINE " << lineNum << endl;
    };
}

//=============================
// MapLoader Implementation
//=============================

MapLoader::MapLoader() = default;

MapLoader::MapLoader(const MapLoader &other) = default;

/**
 * Swap method for copy-and-swap
 * @param a first element
 * @param b second element
 */
void swap(MapLoader &a, MapLoader &b) {}

MapLoader &MapLoader::operator=(MapLoader other) {
    swap(*this, other);
    return *this;
}

ostream &operator<<(ostream &out, const MapLoader &obj) {
    out << "This is a MapLoader!";
    return out;
}

/**
 * Reads a given .map file and return the equivalent Map object
 * @param filePath The path to the .map file
 * @return a Map object
 */
Map *MapLoader::readMapFile(string path, string name) {
    enum class Section {
        files,
        continents,
        countries,
        borders,
        none,
    };

    Section currentSection(Section::none);
    ifstream mapFile(path);
    string line;

    Map *map = new Map(name);
    int lineNum = 0;

    while (getline(mapFile, line)) {
        lineNum++;
        // Clean line to avoid issues with special characters (line carriage, etc.)
        trim(line);

        // Skip empty for comment lines
        if (line.empty() || line[0] == ';') {
            continue;
        }

        // If current line is a header for a section, set currentSection accordingly
        if (line == "[continents]") {
            currentSection = Section::continents;
            continue;
        } else if (line == "[countries]") {
            currentSection = Section::countries;
            continue;
        } else if (line == "[borders]") {
            currentSection = Section::borders;
            continue;
        } else if (line == "[files]") {
            currentSection = Section::files;
            continue;
        }

        // Skip files and none sections
        if (currentSection == Section::files || currentSection == Section::none) {
            continue;
        }

        // Get tokens for the current line
        vector<string> tokens = strSplit(line, " ");
        switch (currentSection) {
            case Section::files: {
                continue;
            }
            case Section::none: {
                continue;
            }
            case Section::continents: {
                // Expected format of line: "ContinentName ArmyValue"
                if (tokens.size() < 2 || !isNumber(tokens[1])) {
                    printError("INVALID CONTINENT", lineNum);
                    return map;
                }
                map->addContinent(tokens[0], stoi(tokens[1]));
                break;
            }
            case Section::countries: {
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
        }
    }
    return map;
}

MapLoader::~MapLoader() = default;

