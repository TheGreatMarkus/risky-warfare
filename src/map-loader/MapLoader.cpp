#include <fstream>
#include <iostream>
#include <vector>
#include <string>

#include "MapLoader.h"
#include "../map/Map.h"

using std::cout;
using std::endl;
using std::ifstream;

namespace {
    string cleanLine(string line) {

        if (line.at(line.size() - 1) == '\r') {
            line.erase(line.size() - 1);
        }
        return line;
    }

    vector<string> strSplit(string str, string token) {
        vector<string> result;
        while (!str.empty()) {
            int index = str.find(token);
            if (index != string::npos) {
                result.push_back(str.substr(0, index));
                str = str.substr(index + token.size());
                if (str.empty())
                    result.push_back(str);
            } else {
                result.push_back(str);
                str = "";
            }
        }
        return result;
    }

} // namespace

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

    while (getline(mapFile, line)) {
        line = cleanLine(line);

        if (line.empty() || line.at(0) == ';') {
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
        }

        if (currentSection == Section::files) {
            continue;
        }

        vector<string> tokens = strSplit(line, " ");
        if (currentSection == Section::continents) {
            map->addContinent(tokens[0], stoi(tokens[1]));
        } else if (currentSection == Section::countries) {
            map->addTerritory(stoi(tokens[0]), tokens[1], stoi(tokens[2]), -1, 0);
        } else if (currentSection == Section::borders) {
            int terr1Id = stoi(tokens.at(0));
            for (int i = 1; i < tokens.size(); i++) {
                int terr2Id = stoi(tokens.at(i));
                map->addConnection(terr1Id, terr2Id);
            }
        }
        cout << line << endl;
    }
    return map;
}

MapLoader::~MapLoader() = default;

