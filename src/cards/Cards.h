#pragma once

#include <vector>
#include <iostream>
#include "../map/Map.h"

using std::vector;
using std::ostream;

class Deck;
class Card;
class Player;
class Hand;
class Order;
class Territory;

/**
 * Deck which contains a finite list of cards.
 * @see Card
 */
class Deck {
private:
    vector<Card *> cards;

public:
    Deck();
    Deck(const Deck &other);
    friend void swap(Deck &a, Deck &b);
    Deck &operator=(Deck other);
    friend ostream &operator<<(ostream &out, const Deck &obj);

    void draw(Hand *hand);
    void addCard(Card *card);

    ~Deck();
};

/**
 * A player's hand which contains a finite set of cards.
 */
class Hand {
private:
    vector<Card *> cards;
public:
    Hand();
    Hand(const Hand &other);
    friend void swap(Hand &a, Hand &b);
    Hand &operator=(Hand other);
    friend ostream &operator<<(ostream &out, const Hand &obj);

    Card *operator[](int i);
    const int size() const;
    const bool empty() const;
    void addCard(Card *card);
    void removeCard(Card *card);
    Card *getCard(int i);
    const vector<Card *> &getCards() const;

    ~Hand();
};

/**
 * An abstract class representing a card a player can play to create special orders.
 */
class Card {
public:
    friend ostream &operator<<(ostream &out, const Card &obj);

    virtual Order *play(Player *cardPlayer,
                        Deck *deck,
                        Map *map,
                        vector<Player *> players) = 0;
    virtual Card *clone() = 0;
    virtual ostream &print(ostream &out) const = 0;
};

/**
 * Card for creating a BombOrder
 * @see BombOrder
 */
class BombCard : public Card {
public:
    Order *play(Player *cardPlayer,
                Deck *deck,
                Map *map,
                vector<Player *> players) override;
    Card *clone() override;
    ostream &print(ostream &out) const override;
};

/**
 * Card for creating a DeployOrder
 * @see DeployOrder
 */
class ReinforcementCard : public Card {
public:
    Order *play(Player *cardPlayer,
                Deck *deck,
                Map *map,
                vector<Player *> players) override;
    Card *clone() override;
    ostream &print(ostream &out) const override;
};

/**
 * Card for creating a BlockadeOrder
 * @see BlockadeOrder
 */
class BlockadeCard : public Card {
public:
    Order *play(Player *cardPlayer,
                Deck *deck,
                Map *map,
                vector<Player *> players) override;
    Card *clone() override;
    ostream &print(ostream &out) const override;
};

/**
 * Card for creating a AirliftOrder
 * @see AirliftOrder
 */
class AirliftCard : public Card {
public:
    Order *play(Player *cardPlayer,
                Deck *deck,
                Map *map,
                vector<Player *> players) override;
    Card *clone() override;
    ostream &print(ostream &out) const override;
};

/**
 * Card for creating a NegotiateOrder
 * @see NegotiateOrder
 */
class DiplomacyCard : public Card {
public:
    Order *play(Player *cardPlayer,
                Deck *deck,
                Map *map,
                vector<Player *> activePlayers) override;
    Card *clone() override;
    ostream &print(ostream &out) const override;
};
