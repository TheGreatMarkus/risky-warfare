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
    Map(string name);
    Map(const Map &other);

    friend void swap(Map &first, Map &second);
    Map &operator=(Map other);
    friend ostream &operator<<(ostream &out, const Map &obj);

    void addTerritory(
        int id,
        string name,
        int continentId,
        int playerId,
        int armies);
    void addContinent(
        int id,
        string name,
        int armyValue);
    void addConnection(int t1Id, int t2Id);
    bool validate();
    Territory *getTerritoryById(int id);
    vector<Territory *> getTerritories();
    vector<Continent *> getContinents();

    ~Map();
};

class Continent
{
private:
    int id;
    string name;
    int armyValue;

public:
    Continent(int id, string name, int armyValue);
    Continent(const Continent &other);

    friend void swap(Continent &first, Continent &second);
    Continent &operator=(Continent other);
    friend ostream &operator<<(ostream &out, const Continent &obj);

    int getId();
    string getName();
    int getArmyValue();

    ~Continent();
};

class Territory
{
private:
    int id;
    string name;
    int continentId;
    int playerId;
    int armies;
    set<int> adj;

public:
    Territory(int id, string name, int continentId, int playerId, int armies);
    Territory(const Territory &other);

    friend void swap(Territory &first, Territory &second);
    Territory &operator=(Territory other);
    friend ostream &operator<<(ostream &out, const Territory &obj);

    set<int> getAdj();
    int getId();
    string getName();

    ~Territory();
};