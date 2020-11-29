
#include "Player.h"
#include "../orders/Orders.h"
#include "../cards/Cards.h"
#include "../map/Map.h"

using std::cout;
using std::endl;

/**
 * Driver for Player
 */
int main() {
    cout << std::boolalpha;

    Deck *deck = new Deck{};

    Player *player1 = new Player{"Player 1"};
    Player *player2 = new Player{"Player 2"};

    player1->addArmies(100);

    Map *map = new Map("test map");
    map->addContinent("c0", 1);
    map->addTerritory("t0", 0, 10);
    map->addTerritory("t1", 0, 10);
    map->addTerritory("t2", 0, 10);
    map->addConnection(0, 1);
    map->addConnection(1, 2);

    Territory *t0 = map->getTerritories()[0];
    Territory *t1 = map->getTerritories()[1];
    Territory *t2 = map->getTerritories()[2];

    player1->captureTerritory(t0);
    player1->captureTerritory(t1);
    player2->captureTerritory(t2);

    auto *bombCard = new BombCard();
    deck->addCard(bombCard);
    deck->draw(player1->getHand());

    player1->issueDeployOrder(map);

    vector<Territory *> toDefend = player1->toDefend(map);
    vector<Territory *> toAttack = player1->toAttack(map);

    player1->issueAdvanceOrder(map, "attack", toAttack);

    cout << *player1;

    delete deck;
    delete map;

    delete player1;
    delete player2;
}
