#include <iostream>
#include <stdexcept>
#include <algorithm>

#include "Map.h"
#include "../utils/Utils.h"


using std::cout;
using std::endl;
using std::runtime_error;
using std::find;

using cris_utils::vectorContains;
using cris_utils::compare;

namespace {
    vector<int> getTerritoriesByContinent(const vector<Territory *> &territories, const int &continent) {
        vector<int> list{};

        for (int i = 0; i < territories.size(); ++i) {
            if (territories[i]->getContinent() == continent) {
                list.push_back(i);
            }
        }
        return list;
    }

    void dfs(int current, vector<Territory *> &territories, vector<set<int>> &adj, vector<bool> &found) {
        found[current] = true;
        for (auto adjId : adj[current]) {
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
        for (auto adjId : adj[current]) {
            if (!vectorContains(found, adjId) && territories[adjId]->getContinent() == continent) {
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
        << "name: \'" << obj.name << "\"" << endl
        << "continents[" << obj.continents.size() << "][" << endl;

    for (const auto &continent : obj.continents) {
        out << "\t" << *continent << endl;
    }

    out << "]" << endl;
    out << "territories[" << obj.territories.size() << "] [" << endl;
    for (const auto &territory : obj.territories) {
        out << "\t" << *territory << endl;
    }

    out << "adj[" << obj.adj.size() << "][" << endl;
    for (int i = 0; i < obj.adj.size(); ++i) {
        out << "\t" << i << ": { ";
        for (const int &adjTerritory : obj.adj[i]) {
            out << adjTerritory << " ";
        }
        out << "}" << endl;
    }

    out << "]" << endl << "}" << endl;
    return out;
}

void Map::addTerritory(
        string name,
        int continent,
        int armies) {
    auto *newTerritory = new Territory(name, continent, armies);
    territories.push_back(newTerritory);
    adj.emplace_back();
}

void Map::addContinent(
        string name,
        int armyValue) {
    auto *newContinent = new Continent(name, armyValue);
    continents.push_back(newContinent);
}

void Map::addConnection(int t1, int t2) {
    if (t1 < adj.size() && t2 < adj.size()) {
        adj[t1].insert(t2);
        adj[t2].insert(t1);
    } else {
        cout << "INVALID CONNECTION: ( " << t1 << ", " << t2 << " )" << endl;
    }

}

bool Map::validate() {
    // Validate that map contains territories anc continents.
    if (continents.empty()) {
        cout << "INVALID MAP: EMPTY CONTINENT LIST" << endl;
        return false;
    }

    if (territories.empty()) {
        cout << "INVALID MAP: EMPTY TERRITORY LIST" << endl;
        return false;
    }

    // Validate map is a connected graph
    {
        vector<bool> found(adj.size());

        dfs(0, territories, adj, found);

        if (vectorContains(found, false)) {
            cout << "INVALID MAP: NOT A CONNECTED GRAPH. THESE TERRITORIES WEREN'T FOUND DURING THE DFS SEARCH: ( ";
            for (int i = 0; i < found.size(); ++i) {
                if (!found[i]) {
                    cout << i << " ";
                }
            }
            cout << ")" << endl;
            return false;
        }
    }

    // Validate each country belongs to one and only one continent.
    for (const auto &territory : territories) {
        if (territory->getContinent() >= continents.size()) {
            cout << "INVALID MAP: " << *territory << " HAS A NON-EXISTENT CONTINENT: " << territory->getContinent() << endl;
            return false;
        }
    }

    // Validate continents are connected sub-graphs
    for (int i = 0; i < continents.size(); ++i) {
        vector<int> continentTerritories = getTerritoriesByContinent(territories, i);
        vector<int> found{};
        dfsContinent(continentTerritories[0], i, territories, adj, found);
        if (!compare(continentTerritories, found)) {
            cout << "INVALID MAP: THE TERRITORIES OF " << *continents[i] << " AREN'T CONNECTED: ( ";
            for (int continentTerritory : continentTerritories) {
                cout << continentTerritory << " ";
            }
            cout << ")" << endl;
            return false;
        }
    }


    return true;
}

const vector<Territory *> &Map::getTerritories() const {
    return territories;
}

bool Map::areAdjacent(int t1, int t2) {
    return adj[t1].find(t2) != adj[t1].end();
}

Map::~Map() {
    for (auto territory : territories) {
        delete territory;
    }
    territories.clear();

    for (auto continent : continents) {
        delete continent;
    }
    territories.clear();
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
        << "name: \"" << obj.name
        << "\", armies: " << obj.armies
        << " }";
    return out;
}

const string &Continent::getName() const {
    return name;
}


Continent::~Continent() = default;

//=============================
// Territory Implementation
//=============================

Territory::Territory(string name, int continent, int armies)
        : name{name},
          continent{continent},
          armies{armies} {}

Territory::Territory(const Territory &other)
        : name{other.name},
          continent{other.continent},
          armies{other.armies} {}

Territory &Territory::operator=(Territory other) {
    swap(*this, other);
    return *this;
}

void swap(Territory &a, Territory &b) {
    using std::swap;

    swap(a.name, b.name);
    swap(a.continent, b.continent);
    swap(a.armies, b.armies);
}

ostream &operator<<(ostream &out, const Territory &obj) {
    out << "Territory{ "
        << "name: \"" << obj.name
        << "\", continent: " << obj.continent
        << ", armies: " << obj.armies
        << " }";

    return out;
}

void Territory::addArmies(int armies) {
    this->armies += armies;
}

void Territory::removeArmies(int armies) {
    if (this->armies < armies) {
        cout << "Removing too many armies!";
    }
    this->armies -= armies;
}

void Territory::bomb() {
    this->armies /= 2;
}

void Territory::blockade() {
    this->armies *= 3;
}

const string &Territory::getName() const {
    return name;
}

const int &Territory::getContinent() const {
    return continent;
}

const int &Territory::getArmies() const {
    return armies;
}

Territory::~Territory() {}










