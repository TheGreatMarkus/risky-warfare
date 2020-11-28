#include "Map.h"

#include <iostream>
#include <stdexcept>
#include <algorithm>

#include "../utils/Utils.h"
#include "../player/Player.h"


using std::cout;
using std::endl;
using std::runtime_error;
using std::find;
using std::pair;
using std::set_difference;
using std::inserter;

using cris_utils::contains;
using cris_utils::compare;
using cris_utils::contains;
using cris_utils::vectorToSet;

namespace {

    /**
     * Perform Depth-first-search on a graph.
     *
     * When encountering a vertex, sets the bool flag for that vertex to true.
     *
     * @param current Current territory index
     * @param territories
     * @param adj
     * @param found List of bool flag for each territory on the map.
     */
    void dfs(Territory *current,
             map<Territory *, set<Territory *>> &adj,
             set<Territory *> &found) {
        found.insert(current);
        for (auto territory : adj[current]) {
            if (!contains(found, territory)) {
                dfs(territory, adj, found);
            }
        }
    }

    /**
     * Performs Depth-first-search of a continent subgraph.
     *
     * Keeps track of which vertices were encountered and stores them in the "found" vector
     * @param current
     * @param continent
     * @param territories
     * @param adj
     * @param found
     */
    void dfsContinent(Territory *current,
                      Continent *continent,
                      set<Territory *> continentTerritories,
                      map<Territory *, set<Territory *>> &adj,
                      set<Territory *> &found) {
        found.insert(current);
        for (auto territory : adj[current]) {
            if (!contains(found, territory) && contains(continentTerritories, territory)) {
                dfsContinent(territory, continent, continentTerritories, adj, found);
            }
        }
    }
}

//=============================
// Map Implementation
//=============================

Map::Map(string name)
        : name{name},
          territories{},
          continents{} {}

/**
 * Copy constructor for Map.
 *
 * Copies all territories and continents to a new list.
 * @param other
 */
Map::Map(const Map &other)
        : name{other.name},
          territories{},
          continents{} {
    for (auto territory : other.territories) {
        territories.push_back(new Territory(*territory));
    }

    for (auto continent : other.continents) {
        continents.push_back(new Continent(*continent));
    }
}

/**
 * Swap method for copy-and-swap
 * @param a first element
 * @param b second element
 */
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

    out << "adj{" << endl;
    for (auto &pair : obj.adj) {
        out << "\t" << pair.first->getName() << ": { ";
        for (Territory *adjTerritory : pair.second) {
            out << adjTerritory->getName() << " ";
        }
        out << "}" << endl;
    }

    out << "}" << endl << "}" << endl;
    return out;
}

void Map::addTerritory(
        string name,
        int continent,
        int armies) {
    auto *newTerritory = new Territory(name, continents[continent], armies);
    territories.push_back(newTerritory);
    if (contains(adj, newTerritory)) {
        adj.insert(pair<Territory *, set<Territory *>>(newTerritory, set<Territory *>{}));
    }
}

void Map::addContinent(
        string name,
        int armyValue) {
    auto *newContinent = new Continent(name, armyValue);
    continents.push_back(newContinent);
}

/**
 * Adds a new edge between two territories.
 * @param t1 The index of the first territory
 * @param t2 The index of the second territory
 */
void Map::addConnection(int t1, int t2) {
    if (t1 < territories.size() && t2 < territories.size()) {
        adj[territories[t1]].insert(territories[t2]);
        adj[territories[t2]].insert(territories[t1]);
    } else {
        cout << "INVALID CONNECTION: ( " << t1 << ", " << t2 << " )" << endl;
    }

}

/**
 * Validates the current map
 * @return Whether the map is valid.
 */
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
        set<Territory *> found{};
        set<Territory *> territoriesSet = vectorToSet(territories);
        dfs(*territories.begin(), adj, found);
        if (!compare(found, territoriesSet)) {
            cout << "INVALID MAP: NOT A CONNECTED GRAPH. THESE TERRITORIES WEREN'T FOUND DURING THE DFS SEARCH: ( ";
            for (auto &territory : territories) {
                if (!contains(found, territory)) {
                    cout << territory->getName() << " ";
                }
            }
            cout << ")" << endl;
            return false;
        }
    }

    // Validate each country belongs to one and only one continent.
    for (const auto &territory : territories) {
        if (!contains(continents, territory->getContinent())) {
            cout << "INVALID MAP: " << *territory << " HAS A NON-EXISTENT CONTINENT: "
                 << territory->getContinent()->getName()
                 << endl;
            return false;
        }
    }

    // Validate continents are connected sub-graphs
    for (auto &continent : continents) {
        set<Territory *> continentTerritories = getTerritoriesByContinent(continent);
        set<Territory *> found{};
        dfsContinent(*continentTerritories.begin(), continent, continentTerritories, adj, found);
        if (!compare(continentTerritories, found)) {
            cout << "INVALID MAP: THE FOLLOWING TERRITORIES OF " << *continent << " AREN'T CONNECTED: ( ";
            for (auto continentTerritory : continentTerritories) {
                if (!contains(found, continentTerritory)) {
                    cout << continentTerritory->getName() << " ";
                }
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


/**
   * Returns all territories that belong to a given continent
   * @param territories
   * @param continent
   * @return list of territories
   */
const set<Territory *> Map::getTerritoriesByContinent(Continent *continent) const {
    set<Territory *> tempSet{};

    for (Territory *territory : territories) {
        if (territory->getContinent() == continent) {
            tempSet.insert(territory);
        }
    }
    return tempSet;
}

/**
 * Returns whether two territories are adjacent.
 * @param t1 The index of the first territory
 * @param t2 The index of the second territory
 * @return Whether the two territories are adjacent
 */
bool Map::areAdjacent(Territory *t1, Territory *t2) {
    return adj[t1].find(t2) != adj[t1].end();
}

set<Continent *> Map::getContinentsControlledByPlayer(Player *player) {
    set<Continent *> controlledContinents{};
    for (auto &continent : continents) {
        set<Territory *> cTerrs = getTerritoriesByContinent(continent);
        set<Territory *> difference;
        set_difference(cTerrs.begin(), cTerrs.end(),
                       player->getOwnedTerritories().begin(), player->getOwnedTerritories().end(),
                       inserter(difference, difference.end()));
        if (difference.empty()) {
            controlledContinents.insert(continent);
        }
    }
    return controlledContinents;
}

const set<Territory *> Map::getNeighbors(Territory *territory) {
    return adj[territory];
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

/**
* Swap method for copy-and-swap
* @param a first element
* @param b second element
*/
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

const int &Continent::getArmies() const {
    return armies;
}


Continent::~Continent() = default;

//=============================
// Territory Implementation
//=============================

Territory::Territory(string name, Continent *continent, int armies)
        : name{name},
          continent{continent},
          armies{0},
          player{nullptr} {}

Territory::Territory(const Territory &other)
        : name{other.name},
          continent{other.continent},
          armies{other.armies},
          player{nullptr} {}

Territory &Territory::operator=(Territory other) {
    swap(*this, other);
    return *this;
}

/**
 * Swap method for copy-and-swap
 * @param a first element
 * @param b second element
 */
void swap(Territory &a, Territory &b) {
    using std::swap;

    swap(a.name, b.name);
    swap(a.continent, b.continent);
    swap(a.armies, b.armies);
    swap(a.player, b.player);
}

ostream &operator<<(ostream &out, const Territory &obj) {
    out << "Territory{ "
        << "name: \"" << obj.name
        << "\", continent: " << obj.continent->getName()
        << ", armies: " << obj.armies;
    if (obj.player != nullptr) {
        out << ", player: " << obj.player->getName();
    }
    out << " }";

    return out;
}

void Territory::addArmies(int armies) {
    this->armies += armies;
}

void Territory::removeArmies(int armies) {
    this->armies -= armies;
    if (this->armies < 0) {
        this->armies = 0;
    }
}

/**
 * Bombs the current territory
 * @see BombOrder
 */
void Territory::bomb() {
    this->armies /= 2;
}

/**
 * Blockades the current territory
 * @see BlockadeOrder
 */
void Territory::blockade() {
    this->armies *= 3;
}

const string &Territory::getName() const {
    return name;
}

const int &Territory::getArmies() const {
    return armies;
}

Continent *Territory::getContinent() const {
    return continent;
}

Player *Territory::getPlayer() {
    return player;
}

void Territory::setContinent(Continent *continent) {
    this->continent = continent;
}

void Territory::setPlayer(Player *player) {
    this->player = player;
}

Territory::~Territory() {}
