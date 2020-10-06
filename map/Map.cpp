#include <iostream>
#include <stdexcept>

#include "Map.h"
#include "../player/Player.h"

using std::cout;
using std::endl;
using std::runtime_error;

Map::Map(string name_)
    : name{name_},
      territories{vector<Territory *>()},
      continents{vector<Continent *>()} {}

Map::Map(const Map &other)
    : name{other.name},
      territories{vector<Territory *>()},
      continents{vector<Continent *>()}
{
    for (auto territory : other.territories)
    {
        Territory *temp = new Territory(*territory);
        territories.push_back(temp);
    }

    for (auto continent : other.continents)
    {
        Continent *temp = new Continent(*continent);
        continents.push_back(temp);
    }
}

Map &Map::operator=(Map other)
{
    swap(*this, other);
    return *this;
}

void swap(Map &first, Map &second)
{
    using std::swap;

    swap(first.name, second.name);
    swap(first.territories, second.territories);
    swap(first.continents, second.continents);
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

void Map::addTerritory(int id, string name, Continent *continent, Player *currentPlayer, int armies)
{
    Territory *newTerritory = new Territory(id, name, continent, currentPlayer, armies);
    territories.push_back(newTerritory);
}

void Map::addConnection(int t1Id, int t2Id)
{
    Territory *t1 = this->getTerritoryById(t1Id);
    Territory *t2 = this->getTerritoryById(t2Id);

    t1->getAdj().insert(t2);
    t2->getAdj().insert(t1);
}

bool Map::validate()
{
    // TODO: Validate map is graph
    // TODO: Validate continents are subgraphs
    // TODO: Check that each country belong to one contient
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

vector<Territory *> &Map::getTerritories()
{
    return territories;
}
vector<Continent *> &Map::getContinents()
{
    return continents;
}

Map::~Map()
{
    cout << "Destroying Map!" << endl;

    for (auto it = territories.begin(); it != territories.end(); ++it)
    {
        delete *it;
    }
    territories.clear();

    cout << "Map destroyed!" << endl;
}

Continent::Continent(string name_, int armyValue_) : name{name_}, armyValue{armyValue_} {}

Continent::Continent(const Continent &other) : name{other.name}, armyValue{other.armyValue} {}

Continent &Continent::operator=(Continent other)
{
    swap(*this, other);
    return *this;
}

void swap(Continent &first, Continent &second)
{
    using std::swap;

    swap(first.name, second.name);
    swap(first.armyValue, second.armyValue);
}

ostream &operator<<(ostream &out, const Continent &obj)
{
    out << "Continent{ name: " << obj.name << ", armyValue: " << obj.armyValue << " }";
    return out;
}

string &Continent::getName()
{
    return name;
}

int &Continent::getArmyValue()
{
    return armyValue;
}

Continent::~Continent() {}

Territory::Territory(int id_, string name_, Continent *continent_, Player *currentPlayer_, int armies_)
    : id{id_},
      name{name_},
      continent{continent_},
      currentPlayer{currentPlayer_},
      armies{armies_},
      adj{set<Territory *>()} {}

Territory::Territory(const Territory &other)
    : id{other.id},
      name{other.name},
      continent{new Continent(*other.continent)},
      currentPlayer{new Player(*other.currentPlayer)},
      armies{other.armies},
      adj{set<Territory *>()} {}

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
    swap(first.continent, second.continent);
    swap(first.currentPlayer, second.currentPlayer);
    swap(first.armies, second.armies);
    swap(first.adj, second.adj);
}

ostream &operator<<(ostream &out, const Territory &obj)
{
    out << "Territory{" << endl;

    out << "\tid: " << obj.id << endl;
    out << "\tname: " << obj.name << endl;
    out << "\tcontinent: " << *obj.continent << endl;
    out << "\tcurrentPlayer: " << *obj.currentPlayer << endl;
    out << "\tarmies: " << obj.armies << endl;
    out << "\tadj[" << obj.adj.size() << "][";

    for (auto const territory : obj.adj)
    {
        out << territory->id << " ";
    }

    out << "]" << endl;
    out << "}" << endl;

    return out;
}

set<Territory *> &Territory::getAdj()
{
    return adj;
}

int &Territory::getId()
{
    return id;
}

string &Territory::getName()
{
    return name;
}

Territory::~Territory()
{
    cout << "Destroying Territory: '" << id << "'" << endl;

    adj.clear();

    cout << "Territory destroyed!" << endl;
}