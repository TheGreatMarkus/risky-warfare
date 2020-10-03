#include <iostream>
#include <stdexcept>
#include "Map.h"

using namespace std;

Map::Map(string id_)
{
    id = new string(id_);
    territories = new set<Territory *>();
}

void Map::addTerritory(string id, Continent continent, string currentPlayer, int armies)
{
    Territory *newTerritory = new Territory(id, continent, currentPlayer, armies);
    territories->insert(newTerritory);
}

void Map::addConnection(Territory *t1, Territory *t2)
{

    t1->getAdj()->insert(t2);
    t2->getAdj()->insert(t1);
}

void Map::validate()
{
    throw runtime_error("Not implemented");
}

void Map::print()
{
    cout << "Number of territories: " << territories->size() << endl;
    int i = 0;
    for (auto const territory : *territories)
    {
        cout << "Territory " << i << ": " << *(territory->getId()) << endl;
        cout << "Connections: ";
        for (auto const adj : *territory->getAdj())
        {
            cout << *(adj->getId()) << ", ";
        }
        i++;
        cout << endl
             << endl;
    }
}

Map::~Map()
{
    cout << "Destroying Map!" << endl;

    delete id;
    id = NULL;

    for (auto it = territories->begin(); it != territories->end(); ++it)
    {
        delete *it;
    }
    territories->clear();
    delete territories;
    territories = NULL;

    cout << "Map destroyed!" << endl;
}

Territory::Territory(string id_, Continent continent_, string currentPlayer_, int armies_)
    : id{new string(id_)},
      continent{new Continent(continent_)},
      currentPlayer{new string(currentPlayer_)},
      armies{new int(armies_)}

{
    adj = new set<Territory *>();
}

Territory::~Territory()
{
    cout << "Destroying Territory: '" << *id << "'" << endl;

    delete id;
    id = NULL;

    delete continent;
    continent = NULL;

    delete currentPlayer;
    currentPlayer = NULL;

    delete armies;
    armies = NULL;

    adj->clear();
    delete adj;
    adj = NULL;

    cout << "Territory destroyed!" << endl;
}