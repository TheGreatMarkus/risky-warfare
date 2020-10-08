#pragma once

#include <vector>

using std::vector;

class Deck;
class Card;
class Player;

class Deck
{
private:
    vector<Card *> *cards;

public:
    Deck(/* args */);

    void draw(Hand *hand);

    vector<Card *> *getCards()
    {
        return cards;
    }

    ~Deck();
};

class Hand
{
private:
    vector<Card *> *cards;

public:
    Hand(/* args */);

    vector<Card *> *getCards();

    ~Hand();
};

class Card
{
private:
    /* data */
public:
    Card(/* args */);

    virtual void play(Player *player, Deck *deck);

    ~Card();
};

class SpyCard
{
private:
    /* data */
public:
    SpyCard(/* args */);
    ~SpyCard();
};

class BombCard
{
private:
    /* data */
public:
    BombCard(/* args */);
    ~BombCard();
};

class ReinforcementCard
{
private:
    /* data */
public:
    ReinforcementCard(/* args */);
    ~ReinforcementCard();
};

class BlockadeCard
{
private:
    /* data */
public:
    BlockadeCard(/* args */);
    ~BlockadeCard();
};

class AirliftCard
{
private:
    /* data */
public:
    AirliftCard(/* args */);
    ~AirliftCard();
};

class DiplomacyCard
{
private:
    /* data */
public:
    DiplomacyCard(/* args */);
    ~DiplomacyCard();
};
