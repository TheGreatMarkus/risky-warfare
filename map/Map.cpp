#include <iostream>
#include <stdexcept>

#include "Map.h"
#include "../player/Player.h"

using std::cout;
using std::endl;
using std::runtime_error;

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
      continents{vector<Continent *>()}
{
    for (auto territory : other.territories)
    {
        territories.push_back(new Territory(*territory));
    }

    for (auto continent : other.continents)
    {
        continents.push_back(new Continent(*continent));
    }
}

void swap(Map &first, Map &second)
{
    using std::swap;

    swap(first.name, second.name);
    swap(first.territories, second.territories);
    swap(first.continents, second.continents);
}

Map &Map::operator=(Map other)
{
    swap(*this, other);
    return *this;
}

ostream &operator<<(ostream &out, const Map &obj)
{
    out << "Map{" << endl
        << "name: " << obj.name << endl;
    out << "continents[" << obj.continents.size() << "][" << endl;
    for (auto const continent : obj.continents)
    {
        out << *continent << endl;
    }
    out << "]" << endl;
    out << "territories[" << obj.territories.size() << "][" << endl;
    for (auto const territory : obj.territories)
    {
        out << *territory;
    }
    out << "]" << endl;
    return out;
}

void Map::addTerritory(
    int id,
    string name,
    int continentId,
    int playerId,
    int armies)
{
    Territory *newTerritory = new Territory(id, name, continentId, playerId, armies);
    territories.push_back(newTerritory);
}

void Map::addContinent(
    int id,
    string name,
    int armyValue)
{
    Continent *newContinent = new Continent(id, name, armyValue);
    continents.push_back(newContinent);
}

void Map::addConnection(int t1Id, int t2Id)
{
    Territory *t1 = this->getTerritoryById(t1Id);
    Territory *t2 = this->getTerritoryById(t2Id);

    t1->getAdj().insert(t2Id);
    t2->getAdj().insert(t1Id);
}

bool Map::validate()
{
    // TODO: Validate map is connected graph
    // TODO: Validate continents are connected subgraphs
    // TODO: Validate each country belongs to one and only one continent.
    return true;
}

Territory *Map::getTerritoryById(int id)
{
    for (Territory *territory : territories)
    {
        if (territory->getId() == id)
        {
            return territory;
        }
    }
    return NULL;
}

vector<Territory *> Map::getTerritories()
{
    return territories;
}
vector<Continent *> Map::getContinents()
{
    return continents;
}

Map::~Map()
{
    cout << "Destroying Map!" << endl;

    for (auto territory : territories)
    {
        delete territory;
    }
    territories.clear();

    for (auto continent : continents)
    {
        delete continent;
    }
    territories.clear();

    cout << "Map destroyed!" << endl;
}

//=============================
// Continent Implementation
//=============================

Continent::Continent(int id, string name, int armyValue)
    : id{id},
      name{name},
      armyValue{armyValue} {}

Continent::Continent(const Continent &other)
    : id{other.id},
      name{other.name},
      armyValue{other.armyValue} {}

void swap(Continent &first, Continent &second)
{
    using std::swap;

    swap(first.id, second.id);
    swap(first.name, second.name);
    swap(first.armyValue, second.armyValue);
}

Continent &Continent::operator=(Continent other)
{
    swap(*this, other);
    return *this;
}

ostream &operator<<(ostream &out, const Continent &obj)
{
    out << "Continent{ id: "
        << obj.id << ", name: "
        << obj.name << ", armyValue: "
        << obj.armyValue << " }";
    return out;
}

int Continent::getId()
{
    return id;
}

string Continent::getName()
{
    return name;
}

int Continent::getArmyValue()
{
    return armyValue;
}

Continent::~Continent() {}

//=============================
// Territory Implementation
//=============================

Territory::Territory(int id, string name, int continentId, int playerId, int armies)
    : id{id},
      name{name},
      continentId{continentId},
      playerId{playerId},
      armies{armies},
      adj{set<int>()} {}

Territory::Territory(const Territory &other)
    : id{other.id},
      name{other.name},
      continentId{other.continentId},
      playerId{other.playerId},
      armies{other.armies},
      adj{set<int>()} {}

Territory &Territory::operator=(Territory other)
{
    swap(*this, other);
    return *this;
}

void swap(Territory &first, Territory &second)
{
    using std::swap;

    swap(first.id, second.id);
    swap(first.name, second.name);
    swap(first.continentId, second.continentId);
    swap(first.playerId, second.playerId);
    swap(first.armies, second.armies);
    swap(first.adj, second.adj);
}

ostream &operator<<(ostream &out, const Territory &obj)
{
    out << "Territory{" << endl;

    out << "\tid: " << obj.id << endl;
    out << "\tname: " << obj.name << endl;
    out << "\tcontinentId: " << obj.continentId << endl;
    out << "\tplayerId: " << obj.playerId << endl;
    out << "\tarmies: " << obj.armies << endl;
    out << "\tadj[" << obj.adj.size() << "][";

    for (auto const territoryId : obj.adj)
    {
        out << territoryId << " ";
    }

    out << "]" << endl;
    out << "}" << endl;

    return out;
}

set<int> Territory::getAdj()
{
    return adj;
}

int Territory::getId()
{
    return id;
}

string Territory::getName()
{
    return name;
}

Territory::~Territory()
{
    // TODO: Verify
    cout << "Destroying Territory: '" << id << "'" << endl;

    adj.clear();

    cout << "Territory destroyed!" << endl;
}