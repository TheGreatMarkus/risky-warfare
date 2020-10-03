#include <set>
#include <string>

#pragma once

using namespace std;

class Map;
class Territory;
class Edge;

class Map
{
private:
    string *id;
    set<Territory *> *territories;

public:
    Map(string id);

    void addTerritory(string id, Continent continent, string currentPlayer, int armies);
    void addConnection(Territory *a1, Territory *a2);
    void validate();
    void print();

    set<Territory *> *getTerritories()
    {
        return territories;
    }

    ~Map();
};

struct Continent
{
    string name;
    int armyValue;
};

class Territory
{
private:
    string *id;
    Continent *continent;
    string *currentPlayer;
    int *armies;
    set<Territory *> *adj;

public:
    Territory(string id, Continent continent, string currentPlayer, int armies);

    set<Territory *> *getAdj()
    {
        return adj;
    }

    string *getId()
    {
        return id;
    }

    ~Territory();
};