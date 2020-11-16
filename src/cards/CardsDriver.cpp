#include "Cards.h"


#include "../player/Player.h"
#include "../map/Map.h"

using std::cout;
using std::endl;

/**
 * Driver for Cards
 * @return
 */
int main() {
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

    auto order1 = hand->getCard(0)->play(deck, hand, t0, t1, 10, player2);
    auto order2 = hand->getCard(0)->play(deck, hand, t0, t1, 10, player2);
    auto order3 = hand->getCard(0)->play(deck, hand, t0, t1, 10, player2);
    auto order4 = hand->getCard(0)->play(deck, hand, t0, t1, 10, player2);
    auto order5 = hand->getCard(0)->play(deck, hand, t0, t1, 10, player2);

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