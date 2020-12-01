#include "Cards.h"

#include "../player/Player.h"
#include "../orders/Orders.h"

using std::cout;
using std::endl;

/**
 * Driver for Cards
 * @return
 */
int main() {
    cout << std::boolalpha;

    Deck *deck = new Deck{};

    vector<Player *> players;
    players.push_back(new Player("Player 1"));
    players.push_back(new Player("Player 2"));

    Map *map = new Map("test map");
    map->addContinent("c0", 1);
    map->addTerritory("t0", 0, 10);
    map->addTerritory("t1", 0, 10);
    map->addTerritory("t2", 0, 10);
    map->addConnection(0, 1);
    map->addConnection(1, 2);

    players[0]->captureTerritory(map->getTerritories()[0]);
    players[0]->captureTerritory(map->getTerritories()[1]);
    players[1]->captureTerritory(map->getTerritories()[2]);

    deck->addCard(new BombCard());
    deck->addCard(new ReinforcementCard());
    deck->addCard(new BlockadeCard());
    deck->addCard(new AirliftCard());
    deck->addCard(new DiplomacyCard());

    Hand *hand = players[0]->getHand();
    deck->draw(hand);
    deck->draw(hand);
    deck->draw(hand);
    deck->draw(hand);
    deck->draw(hand);

    auto order1 = hand->getCard(0)->play(players[0], deck, map, players);
    auto order2 = hand->getCard(0)->play(players[0], deck, map, players);
    auto order3 = hand->getCard(0)->play(players[0], deck, map, players);
    auto order4 = hand->getCard(0)->play(players[0], deck, map, players);
    auto order5 = hand->getCard(0)->play(players[0], deck, map, players);

    cout << endl;
    cout << *deck << endl;
    cout << *hand << endl;

    cout << *order1 << endl;
    cout << *order2 << endl;
    cout << *order3 << endl;
    cout << *order4 << endl;
    cout << *order5 << endl;

    delete deck;

    for (auto &player : players) {
        delete player;
    }

    delete map;

    delete order1;
    delete order2;
    delete order3;
    delete order4;
    delete order5;
}
