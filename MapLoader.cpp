#include "MapLoader.h"
#include <fstream>
#include <iostream>
#include <algorithm>
#include <vector>

using namespace std;

namespace
{
    string cleanLine(string line)
    {

        if (line.at(line.size() - 1) == '\r')
        {
            line.erase(line.size() - 1);
        }
        return line;
    }

} // namespace

MapLoader::MapLoader(/* args */)
{
}

void MapLoader::readMapFile(string fileName)
{
    enum class Section
    {
        continents,
        countries,
        borders,
        none,
    };

    Section currentSection{Section::none};
    ifstream mapFile(fileName);
    string line;

    vector<Continent>

        while (getline(mapFile, line))
    {
        line = cleanLine(line);

        if (line.size() == 0 || line.at(0) == ';')
        {
            continue;
        }

        // Set mode
        if (line == "[continents]")
        {
            currentSection = Section::continents;
        }
        else if (line == "[countries]")
        {
            currentSection = Section::countries;
        }
        else if (line == "[borders]")
        {
            currentSection = Section::borders;
        }
        cout << line << endl;
    }
}

MapLoader::~MapLoader()
{
}