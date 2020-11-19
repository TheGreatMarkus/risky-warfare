#include "Cards.h"

#include <iostream>

#include "../player/Player.h"
#include "../orders/Orders.h"
#include "../map/Map.h"

using std::cout;
using std::endl;

/**
 * Driver for Cards
 * @return
 */
int main() {
    cout << std::boolalpha;

    Deck *deck = new Deck{};

    Player *player1 = new Player{"p0"};
    Player *player2 = new Player{"p0"};

    Hand *hand = player1->getHand();

    Continent *c0 = new Continent{"c0", 1};
    Territory *t0 = new Territory{"t0", c0, 10};
    Territory *t1 = new Territory{"t1", c0, 10};

    deck->addCard(new BombCard());
    deck->addCard(new ReinforcementCard());
    deck->addCard(new BlockadeCard());
    deck->addCard(new AirliftCard());
    deck->addCard(new DiplomacyCard());

    deck->draw(hand);
    deck->draw(hand);
    deck->draw(hand);
    deck->draw(hand);
    deck->draw(hand);

    auto order1 = hand->getCard(0)->play(nullptr, deck, nullptr, vector<Player*>());
    auto order2 = hand->getCard(0)->play(nullptr, deck, nullptr, vector<Player*>());
    auto order3 = hand->getCard(0)->play(nullptr, deck, nullptr, vector<Player*>());
    auto order4 = hand->getCard(0)->play(nullptr, deck, nullptr, vector<Player*>());
    auto order5 = hand->getCard(0)->play(nullptr, deck, nullptr, vector<Player*>());

    cout << *deck << endl;
    cout << *hand << endl;

    delete deck;

    delete player1;
    delete player2;

    delete order1;
    delete order2;
    delete order3;
    delete order4;
    delete order5;
}