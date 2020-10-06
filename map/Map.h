#pragma once

#include <set>
#include <string>
#include <vector>

using std::ostream;
using std::set;
using std::string;
using std::vector;

class Map;
class Territory;
class Continent;
class Player;

class Map
{
private:
    string name;
    vector<Territory *> territories;
    vector<Continent *> continents;

public:
    Map(string name_);
    Map(const Map &other);

    Map &operator=(Map other);
    friend void swap(Map &first, Map &second);
    friend ostream &operator<<(ostream &out, const Map &obj);

    void addTerritory(int id, string name, Continent *continent, Player *currentPlayer, int armies);
    void addConnection(int t1Id, int t2Id);
    bool validate();
    Territory *getTerritoryById(int id);
    vector<Territory *> &getTerritories();
    vector<Continent *> &getContinents();

    ~Map();
};

class Continent
{
private:
    string name;
    int armyValue;

public:
    Continent(string name_, int armyValue_);
    Continent(const Continent &other);

    Continent &operator=(Continent other);
    friend void swap(Continent &first, Continent &second);
    friend ostream &operator<<(ostream &out, const Continent &obj);

    string &getName();
    int &getArmyValue();

    ~Continent();
};

class Territory
{
private:
    int id;
    string name;
    Continent *continent;
    Player *currentPlayer;
    int armies;
    set<Territory *> adj;

public:
    Territory(int id, string name, Continent *continent, Player *currentPlayer, int armies);
    Territory(const Territory &other);

    Territory &operator=(Territory other);
    friend void swap(Territory &first, Territory &second);
    friend ostream &operator<<(ostream &out, const Territory &obj);

    set<Territory *> &getAdj();
    int &getId();
    string &getName();

    ~Territory();
};