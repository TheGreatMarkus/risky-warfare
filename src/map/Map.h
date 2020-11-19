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

/**
 * Class which represents a connected graph of territories.
 */
class Map {
private:
    string name;
    vector<Territory *> territories;
    vector<Continent *> continents;
    map<Territory *, set<Territory *>> adj;

public:
    Map(string name);
    Map(const Map &other);
    friend void swap(Map &a, Map &b);
    Map &operator=(Map other);
    friend ostream &operator<<(ostream &out, const Map &obj);

    void addTerritory(string name,
                      int continent,
                      int armies);
    void addContinent(string name, int armyValue);
    void addConnection(int t1, int t2);

    bool validate();
    const vector<Territory *> &getTerritories() const;
    const set<Territory *> getTerritoriesByContinent(Continent *continent) const;
    bool areAdjacent(Territory *t1, Territory *t2);
    set<Continent *> getContinentsControlledByPlayer(Player *player);
    const set<Territory *> getNeighbors(Territory *territory) ;


    ~Map();
};

/**
 * Class for representing a Continent, which is associated with a subset of the territories
 */
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
    const int &getArmies() const;

    ~Continent();
};

/**
 * Class for representing a Territory on a Map.
 */
class Territory {
private:
    string name;
    int armies;
    Continent *continent;
    Player *player;

public:
    Territory(string name,
              Continent *continent,
              int armies);
    Territory(const Territory &other);
    friend void swap(Territory &a, Territory &b);
    Territory &operator=(Territory other);
    friend ostream &operator<<(ostream &out, const Territory &obj);

    void addArmies(int armies);
    void removeArmies(int armies);
    void bomb();
    void blockade();

    const string &getName() const;
    const int &getArmies() const;
    Continent *getContinent() const;
    Player *getPlayer();

    void setContinent(Continent *continent);
    void setPlayer(Player *player);

    ~Territory();
};