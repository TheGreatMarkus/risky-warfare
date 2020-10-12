#include <fstream>
#include <iostream>
#include <vector>
#include <string>

#include "MapLoader.h"
#include "../map/Map.h"
#include "../utils/Utils.h"

using std::cout;
using std::endl;
using std::ifstream;

using cris_utils::strSplit;
using cris_utils::trim;
using cris_utils::isNumber;
using cris_utils::NO_PLAYER_ID;

namespace {
    void printError(string message, int lineNum) {
        cout << "ERROR: \"" << message << "\" AT LINE " << lineNum << endl;
    };
}

MapLoader::MapLoader() = default;

MapLoader::MapLoader(const MapLoader &other) = default;

void swap(MapLoader &a, MapLoader &b) {}

MapLoader &MapLoader::operator=(MapLoader other) {
    swap(*this, other);
    return *this;
}

ostream &operator<<(ostream &out, const MapLoader &obj) {
    out << "This is a MapLoader!";
    return out;
}

Map *MapLoader::readMapFile(string fileName) {
    enum class Section {
        files,
        continents,
        countries,
        borders,
        none,
    };

    Section currentSection(Section::none);
    ifstream mapFile(fileName);
    string line;

    Map *map = new Map(fileName);
    int lineNum = 0;

    while (getline(mapFile, line)) {
        lineNum++;
        trim(line);

        if (line.empty() || line[0] == ';') {
            continue;
        }

        // Set mode
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

        if (currentSection == Section::files) {
            continue;
        }

        vector<string> tokens = strSplit(line, " ");
        switch (currentSection) {
            case Section::files:
            case Section::none:
                continue;
            case Section::continents:
                if (tokens.size() < 2 || !isNumber(tokens[1])) {
                    printError("INVALID CONTINENT", lineNum);
                    return map;
                }
                map->addContinent(tokens[0], stoi(tokens[1]));
                break;
            case Section::countries:
                if (tokens.size() < 3 || !isNumber(tokens[0]) || !isNumber(tokens[2])) {
                    printError("INVALID COUNTRY/TERRITORY", lineNum);
                    return map;
                }
                map->addTerritory(tokens[1], stoi(tokens[2]) - 1, NO_PLAYER_ID, 0);
                break;
            case Section::borders:
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
    return map;
}

MapLoader::~MapLoader() = default;

