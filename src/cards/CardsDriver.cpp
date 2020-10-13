#include "Cards.h"

/**
 * Driver for Cards
 * @return
 */
int main() {
    auto deck = new Deck{};
    auto hand = new Hand{};

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

    auto order1 = hand->getCard(0)->play(deck, hand, 0, 1, 10, 0);
    auto order2 = hand->getCard(0)->play(deck, hand, 0, 1, 10, 0);
    auto order3 = hand->getCard(0)->play(deck, hand, 0, 1, 10, 0);
    auto order4 = hand->getCard(0)->play(deck, hand, 0, 1, 10, 0);
    auto order5 = hand->getCard(0)->play(deck, hand, 0, 1, 10, 0);

    delete deck;
    delete hand;

    delete order1;
    delete order2;
    delete order3;
    delete order4;
    delete order5;
}