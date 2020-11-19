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
    Card();
    Card(const Card &other);
    friend void swap(Card &a, Card &b);
    //Card &operator=(Card other); // abstract class, can't overload assignment operator
    friend ostream &operator<<(ostream &out, const Card &obj);

    virtual Order *play(Player *cardPlayer, Deck *deck, Map *map, vector<Player *> players) = 0;
    virtual Card *clone() = 0;
    virtual ostream &print(ostream &out) const = 0;

    virtual ~Card() = 0;
};

/**
 * Card for creating a BombOrder
 * @see BombOrder
 */
class BombCard : public Card {
public:
    BombCard();
    BombCard(const BombCard &other);
    friend void swap(BombCard &a, BombCard &b);
    BombCard &operator=(BombCard other);
    friend ostream &operator<<(ostream &out, const BombCard &obj);

    Order *play(Player *cardPlayer, Deck *deck, Map *map, vector<Player *> players) override;
    Card *clone() override;
    ostream &print(ostream &out) const override;

    ~BombCard() override;
};

/**
 * Card for creating a DeployOrder
 * @see DeployOrder
 */
class ReinforcementCard : public Card {
public:
    ReinforcementCard();
    ReinforcementCard(const ReinforcementCard &other);
    friend void swap(ReinforcementCard &a, ReinforcementCard &b);
    ReinforcementCard &operator=(ReinforcementCard other);
    friend ostream &operator<<(ostream &out, const ReinforcementCard &obj);

    Order *play(Player *cardPlayer, Deck *deck, Map *map, vector<Player *> players) override;
    Card *clone() override;
    ostream &print(ostream &out) const override;

    ~ReinforcementCard() override;
};

/**
 * Card for creating a BlockadeOrder
 * @see BlockadeOrder
 */
class BlockadeCard : public Card {
public:
    BlockadeCard();
    BlockadeCard(const BlockadeCard &other);
    friend void swap(BlockadeCard &a, BlockadeCard &b);
    BlockadeCard &operator=(BlockadeCard other);
    friend ostream &operator<<(ostream &out, const BlockadeCard &obj);

    Order *play(Player *cardPlayer, Deck *deck, Map *map, vector<Player *> players) override;
    Card *clone() override;
    ostream &print(ostream &out) const override;

    ~BlockadeCard() override;
};

/**
 * Card for creating a AirliftOrder
 * @see AirliftOrder
 */
class AirliftCard : public Card {
public:
    AirliftCard();
    AirliftCard(const AirliftCard &other);
    friend void swap(AirliftCard &a, AirliftCard &b);
    AirliftCard &operator=(AirliftCard other);
    friend ostream &operator<<(ostream &out, const AirliftCard &obj);

    Order *play(Player *cardPlayer, Deck *deck, Map *map, vector<Player *> players) override;
    Card *clone() override;
    ostream &print(ostream &out) const override;

    ~AirliftCard() override;
};

/**
 * Card for creating a NegotiateOrder
 * @see NegotiateOrder
 */
class DiplomacyCard : public Card {
public:
    DiplomacyCard();
    DiplomacyCard(const DiplomacyCard &other);
    friend void swap(DiplomacyCard &a, DiplomacyCard &b);
    DiplomacyCard &operator=(DiplomacyCard other);
    friend ostream &operator<<(ostream &out, const DiplomacyCard &obj);

    Order *play(Player *cardPlayer, Deck *deck, Map *map, vector<Player *> activePlayers) override;
    Card *clone() override;
    ostream &print(ostream &out) const override;

    ~DiplomacyCard() override;
};
