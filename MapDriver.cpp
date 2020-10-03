#include <iostream>
#include <string>
#include <vector>
#include "Map.h"

int main()
{
    Map newMap = Map("newMap");

    newMap.addTerritory("Territory 1", {"a", 0}, "", 0);
    newMap.addTerritory("Territory 2", {"a", 0}, "", 0);
    newMap.addTerritory("Territory 3", {"a", 0}, "", 0);
    newMap.addTerritory("Territory 4", {"a", 0}, "", 0);

    vector<Territory *> territories(begin(*newMap.getTerritories()), end(*newMap.getTerritories()));

    newMap.addConnection(territories.at(0), territories.at(1));
    newMap.addConnection(territories.at(0), territories.at(2));
    newMap.addConnection(territories.at(0), territories.at(3));
    newMap.addConnection(territories.at(2), territories.at(3));

    newMap.print();

    // newMap.validate();
    return 0;
}