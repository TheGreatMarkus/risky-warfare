#pragma once

#include <vector>
#include <iostream>

using std::vector;
using std::ostream;

class Deck;
class Card;
class Player;
class Hand;

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

class Hand {
private:
    vector<Card *> cards;

public:
    Hand();
    Hand(const Hand &other);
    friend void swap(Hand &a, Hand &b);
    Hand &operator=(Hand other);
    friend ostream &operator<<(ostream &out, const Hand &obj);

    void addCard(Card *card);
    vector<Card *> getCards() const;

    ~Hand();
};

class Card {
private:
    /* data */
public:
    Card();
    Card(const Card &other);
    // friend void swap(Card &a, Card &b);
    // Can't override assignment operator for abstract class
    // Card &operator=(Card other);
    friend ostream &operator<<(ostream &out, const Card &obj);

    virtual void play(Player *player, Deck *deck) = 0;

    ~Card();
};

class BombCard : public Card {
private:
    /* data */
public:
    BombCard();
    BombCard(const BombCard &other);
    friend void swap(BombCard &a, BombCard &b);
    BombCard &operator=(BombCard other);
    friend ostream &operator<<(ostream &out, const BombCard &obj);

    void play(Player *player, Deck *deck) override;

    ~BombCard();
};

class ReinforcementCard : public Card {
private:
    /* data */
public:
    ReinforcementCard();
    ReinforcementCard(const ReinforcementCard &other);
    friend void swap(ReinforcementCard &a, ReinforcementCard &b);
    ReinforcementCard &operator=(ReinforcementCard other);
    friend ostream &operator<<(ostream &out, const ReinforcementCard &obj);

    void play(Player *player, Deck *deck) override;

    ~ReinforcementCard();
};

class BlockadeCard : public Card {
private:
    /* data */
public:
    BlockadeCard();
    BlockadeCard(const BlockadeCard &other);
    friend void swap(BlockadeCard &a, BlockadeCard &b);
    BlockadeCard &operator=(BlockadeCard other);
    friend ostream &operator<<(ostream &out, const BlockadeCard &obj);

    void play(Player *player, Deck *deck) override;

    ~BlockadeCard();
};

class AirliftCard : public Card {
private:
    /* data */
public:
    AirliftCard();
    AirliftCard(const AirliftCard &other);
    friend void swap(AirliftCard &a, AirliftCard &b);
    AirliftCard &operator=(AirliftCard other);
    friend ostream &operator<<(ostream &out, const AirliftCard &obj);

    void play(Player *player, Deck *deck) override;

    ~AirliftCard();
};

class DiplomacyCard : public Card {
private:
    /* data */
public:
    DiplomacyCard();
    DiplomacyCard(const DiplomacyCard &other);
    friend void swap(DiplomacyCard &a, DiplomacyCard &b);
    DiplomacyCard &operator=(DiplomacyCard other);
    friend ostream &operator<<(ostream &out, const DiplomacyCard &obj);

    void play(Player *player, Deck *deck) override;

    ~DiplomacyCard();
};
