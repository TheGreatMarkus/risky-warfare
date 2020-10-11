#include <iostream>
#include <stdexcept>
#include <algorithm>

#include "Map.h"
#include "../player/Player.h"
#include "../orders/Orders.h"


using std::cout;
using std::endl;
using std::runtime_error;
using std::find;

namespace {


    template<class T>
    bool vectorContains(vector<T> v, T element) {
        return find(v.begin(), v.end(), element) != v.end();
    }

    vector<int> getTerritoriesByContinent(const vector<Territory *> &territories, const int &continent) {
        vector<int> list{};

        for (int i = 0; i < territories.size(); ++i) {
            if (territories.at(i)->getContinent() == continent) {
                list.push_back(i);
            }
        }
        return list;
    }

    void dfs(int current, vector<Territory *> &territories, vector<set<int>> &adj, vector<bool> &found) {
        found[current] = true;
        for (auto adjId : adj.at(current)) {
            if (!found[adjId]) {
                dfs(adjId, territories, adj, found);
            }
        }
    }

    void dfsContinent(int current,
                      int continent,
                      vector<Territory *> &territories,
                      vector<set<int>> &adj,
                      vector<int> &found) {
        found.push_back(current);
        for (auto adjId : adj.at(current)) {
            if (!vectorContains(found, adjId) && territories.at(adjId)->getContinent() == continent) {
                dfsContinent(adjId, continent, territories, adj, found);
            }
        }
    }
}

//=============================
// Map Implementation
//=============================

Map::Map(string name)
        : name{name},
          territories{vector<Territory *>()},
          continents{vector<Continent *>()} {}

Map::Map(const Map &other)
        : name{other.name},
          territories{vector<Territory *>()},
          continents{vector<Continent *>()} {
    for (auto territory : other.territories) {
        territories.push_back(new Territory(*territory));
    }

    for (auto continent : other.continents) {
        continents.push_back(new Continent(*continent));
    }
}

void swap(Map &a, Map &b) {
    using std::swap;

    swap(a.name, b.name);
    swap(a.territories, b.territories);
    swap(a.continents, b.continents);
}


Map &Map::operator=(Map other) {
    swap(*this, other);
    return *this;
}

ostream &operator<<(ostream &out, const Map &obj) {
    out << "Map{" << endl
        << "name: " << obj.name << endl;
    out << "continents[" << obj.continents.size() << "][" << endl;
    for (auto const continent : obj.continents) {
        out << *continent << endl;
    }
    out << "]" << endl;
    out << "territories[" << obj.territories.size() << "][" << endl;
    for (auto const territory : obj.territories) {
        out << *territory;
    }
    out << "]" << endl;
    return out;
}

void Map::addTerritory(
        string terrName,
        int continent,
        int playerId,
        int armies) {
    Territory *newTerritory = new Territory(terrName, continent, playerId, armies);
    territories.push_back(newTerritory);
    adj.push_back(set<int>());
}

void Map::addContinent(
        string continentName,
        int armyValue) {
    Continent *newContinent = new Continent(continentName, armyValue);
    continents.push_back(newContinent);
}

void Map::addConnection(int t1Id, int t2Id) {
    adj[t1Id - 1].insert(t2Id - 1);
    adj[t2Id - 1].insert(t1Id - 1);
}

bool Map::validate() {
    // TODO: Mine, all territories and continent ids are unique and increment from 1

    if (territories.empty() || continents.empty()) {
        cout << "INVALID SIZE" << endl;
    }

    // Validate map is connected graph
    {
        vector<bool> found(adj.size());

        dfs(0, territories, adj, found);

        if (vectorContains(found, false)) {
            cout << "NOT CONNECTED GRAPH" << endl;
        }
    }

    // TODO: Validate continents are connected subgraphs
    for (int i = 0; i < continents.size(); ++i) {
        vector<int> continentTerritories = getTerritoriesByContinent(territories, i);
        vector<int> found{};
        dfsContinent(0, i, territories, adj, found);
        cout << "result: " << found.size() << endl;
    }

    // TODO: Validate each country belongs to one and only one continent.


    return true;
}


vector<Territory *> Map::getTerritories() {
    return territories;
}

vector<Continent *> Map::getContinents() {
    return continents;
}

Map::~Map() {
    cout << "Destroying Map!" << endl;

    for (auto territory : territories) {
        delete territory;
    }
    territories.clear();

    for (auto continent : continents) {
        delete continent;
    }
    territories.clear();

    cout << "Map destroyed!" << endl;
}



//=============================
// Continent Implementation
//=============================

Continent::Continent(string name, int armyValue)
        : name{name},
          armies{armyValue} {}

Continent::Continent(const Continent &other)
        : name{other.name},
          armies{other.armies} {}

void swap(Continent &a, Continent &b) {
    using std::swap;

    swap(a.name, b.name);
    swap(a.armies, b.armies);
}

Continent &Continent::operator=(Continent other) {
    swap(*this, other);
    return *this;
}

ostream &operator<<(ostream &out, const Continent &obj) {
    out << "Continent{ "
        << ", name: " << obj.name
        << ", armies: " << obj.armies
        << " }";
    return out;
}

string Continent::getName() {
    return name;
}

int Continent::getArmyValue() {
    return armies;
}


Continent::~Continent() = default;

//=============================
// Territory Implementation
//=============================

Territory::Territory(string name, int continent, int playerId, int armies)
        : name{name},
          continent{continent},
          playerId{playerId},
          armies{armies} {}

Territory::Territory(const Territory &other)
        :
        name{other.name},
        continent{other.continent},
        playerId{other.playerId},
        armies{other.armies} {}

Territory &Territory::operator=(Territory other) {
    swap(*this, other);
    return *this;
}

void swap(Territory &a, Territory &b) {
    using std::swap;

    swap(a.name, b.name);
    swap(a.continent, b.continent);
    swap(a.playerId, b.playerId);
    swap(a.armies, b.armies);
}

ostream &operator<<(ostream &out, const Territory &obj) {
    out << "Territory{" << endl;

    out << "name: " << obj.name << endl;
    out << "continentId: " << obj.continent << endl;
    out << "playerId: " << obj.playerId << endl;
    out << "armies: " << obj.armies << endl;

    out << "}" << endl;

    return out;
}


string Territory::getName() {
    return name;
}

int Territory::getContinent() {
    return continent;
}

Territory::~Territory() {}


