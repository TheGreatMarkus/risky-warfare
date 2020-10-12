#pragma once

#include <set>
#include <string>
#include <vector>
#include <map>

using std::map;
using std::ostream;
using std::set;
using std::string;
using std::vector;


class Map;
class Territory;
class Continent;
class Player;


class Map {
private:
    string name;
    vector<Territory *> territories;
    vector<Continent *> continents;
    vector<set<int>> adj;

public:
    Map(string name);
    Map(const Map &other);
    friend void swap(Map &a, Map &b);
    Map &operator=(Map other);
    friend ostream &operator<<(ostream &out, const Map &obj);

    void addTerritory(string terrName,
                      int continent,
                      int playerId,
                      int armies);
    void addContinent(string continentName, int armyValue);
    void addConnection(int t1Id, int t2Id);
    bool validate();

    ~Map();
};

class Continent {
private:
    string name;
    int armies;

public:
    Continent(string name, int armyValue);
    Continent(const Continent &other);
    friend void swap(Continent &a, Continent &b);
    Continent &operator=(Continent other);
    friend ostream &operator<<(ostream &out, const Continent &obj);

    const string &getName() const;

    ~Continent();
};

const int NO_PLAYER_ID = -1;

class Territory {
private:
    string name;
    int continent;
    int playerId;
    int armies;

public:
    Territory(string name,
              int continent,
              int playerId,
              int armies);
    Territory(const Territory &other);
    friend void swap(Territory &a, Territory &b);
    Territory &operator=(Territory other);
    friend ostream &operator<<(ostream &out, const Territory &obj);

    const string &getName() const;
    const int &getContinent() const;

    ~Territory();
};