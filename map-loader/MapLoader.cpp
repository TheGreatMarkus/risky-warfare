#include "MapLoader.h"
#include <fstream>
#include <iostream>
#include <algorithm>
#include <vector>
#include <sstream>
#include "../player/Player.h"

using std::cout;
using std::endl;
using std::ifstream;

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

    vector<string> strSplit(string str, string token)
    {
        vector<string> result;
        while (str.size())
        {
            int index = str.find(token);
            if (index != string::npos)
            {
                result.push_back(str.substr(0, index));
                str = str.substr(index + token.size());
                if (str.size() == 0)
                    result.push_back(str);
            }
            else
            {
                result.push_back(str);
                str = "";
            }
        }
        return result;
    }

} // namespace

Map *readMapFile(string fileName)
{
    enum class Section
    {
        files,
        continents,
        countries,
        borders,
        none,
    };

    Section currentSection{Section::none};
    ifstream mapFile(fileName);
    string line;

    Map *map = new Map(fileName);

    Player *player1 = new Player("asdfs");

    vector<Continent *>
        continents = map->getContinents();
    vector<Territory *> territories = map->getTerritories();

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
            continue;
        }
        else if (line == "[countries]")
        {
            currentSection = Section::countries;
            continue;
        }
        else if (line == "[borders]")
        {
            currentSection = Section::borders;
            continue;
        }
        else if (line == "[files]")
        {
            currentSection = Section::files;
        }

        if (currentSection == Section::files)
        {
            continue;
        }

        vector<string> tokens = strSplit(line, " ");
        if (currentSection == Section::continents)
        {
            continents.push_back(new Continent{tokens[0], stoi(tokens[1])});
        }
        else if (currentSection == Section::countries)
        {
            map->addTerritory(stoi(tokens[0]), tokens[1], continents.at(stoi(tokens[2]) - 1), player1, 0);
        }
        else if (currentSection == Section::borders)
        {
            int terr1Id = stoi(tokens.at(0));
            for (int i = 1; i < tokens.size(); i++)
            {
                int terr2Id = stoi(tokens.at(i));
                map->addConnection(terr1Id, terr2Id);
            }
        }
        cout << line << endl;
    }
    return map;
}
