#include "Cards.h"

//=============================
// Deck Implementation
//=============================

Deck::Deck()
        : cards{vector<Card *>()} {}

Deck::Deck(const Deck &other)
        : cards{vector<Card *>{}} {
    // TODO: Copy
}

void swap(Deck &a, Deck &b) {
    using std::swap;

    swap(a.cards, b.cards);
}

Deck &Deck::operator=(Deck other) {
    swap(*this, other);
    return *this;
}

ostream &operator<<(ostream &out, const Deck &obj) {
    // TODO
    out << "";
    return out;
}

void Deck::draw(Hand *hand) {
    int picked = rand() % cards.size();

    Card *pickedCard = cards.at(picked);
    cards.erase(cards.begin() + picked);

    hand->addCard(pickedCard);
}

void Deck::addCard(Card *card) {
    cards.push_back(card);
}

Deck::~Deck() {}

//=============================
// Hand Implementation
//=============================



Hand::Hand() : cards{vector<Card *>()} {}

Hand::Hand(const Hand &other) : cards{vector<Card *>()} {
    // TODO copy cards in vector
}

void swap(Hand &a, Hand &b) {
    using std::swap;
    swap(a.cards, b.cards);
}

Hand &Hand::operator=(Hand other) {
    swap(*this, other);
    return *this;
}

ostream &operator<<(ostream &out, const Hand &obj) {
    // TODO
    out << "";
    return out;
}

void Hand::addCard(Card *card) {
    cards.push_back(card);
}

vector<Card *> Hand::getCards() const {
    return cards;
}

Hand::~Hand() {}

//=============================
// Card Implementation
//=============================

Card::Card() {}

Card::Card(const Card &other) {}

ostream &operator<<(ostream &out, const Card &obj) {
    out << "";
    return out;
}

Card::~Card() {}


