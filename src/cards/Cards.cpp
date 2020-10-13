#include <algorithm>

#include "Cards.h"
#include "../orders/Orders.h"
#include "../player/Player.h"
#include "../utils/Utils.h"

using cris_utils::removeElement;
using std::endl;
using std::remove;
//=============================
// Deck Implementation
//=============================

Deck::Deck()
        : cards{vector<Card *>()} {}

Deck::Deck(const Deck &other)
        : cards{vector<Card *>{}} {
    for (const auto &card : other.cards) {
        cards.push_back(card->clone());
    }
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
    out << "Deck{ " << endl
        << "cards[" << obj.cards.size() << "][" << endl;
    for (auto card : obj.cards) {
        out << *card << endl;
    }
    out << "]" << endl << "}" << endl;

    return out;
}

void Deck::draw(Hand *hand) {
    int picked = rand() % cards.size();

    Card *pickedCard = cards[picked];
    cards.erase(cards.begin() + picked);

    hand->addCard(pickedCard);
}

void Deck::addCard(Card *card) {
    cards.push_back(card);
}

Deck::~Deck() {
    for (auto card : cards) {
        delete card;
    }
    cards.clear();
}

//=============================
// Hand Implementation
//=============================



Hand::Hand() : cards{vector<Card *>()} {}

Hand::Hand(const Hand &other) : cards{vector<Card *>()} {
    for (const auto &card : other.cards) {
        cards.push_back(card->clone());
    }
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
    out << "Hand{ " << endl
        << "cards[" << obj.cards.size() << "][" << endl;
    for (auto card : obj.cards) {
        out << *card << endl;
    }
    out << "]" << endl << "}" << endl;

    return out;
}

void Hand::addCard(Card *card) {
    cards.push_back(card);
}

void Hand::removeCard(Card *card) {
    cards.erase(remove(cards.begin(), cards.end(), card), cards.end());
}

Card *Hand::getCard(int i) {
    return cards[i];
}


Hand::~Hand() {
    for (auto card : cards) {
        delete card;
    }
    cards.clear();
}




//=============================
// Card Implementation
//=============================

Card::Card() {}

Card::Card(const Card &other) {}

void swap(Card &a, Card &b) {
    using std::swap;
}

ostream &operator<<(ostream &out, const Card &obj) {
    obj.print(out);
    return out;
}

Card::~Card() {}

//=============================
// BombCard Implementation
//=============================
BombCard::BombCard() {}

BombCard::BombCard(const BombCard &other) {}

void swap(BombCard &a, BombCard &b) {
    using std::swap;
}

BombCard &BombCard::operator=(BombCard other) {
    swap(*this, other);
    return *this;
}

ostream &operator<<(ostream &out, const BombCard &obj) {
    obj.print(out);
    return out;
}

Order *BombCard::play(Deck *deck, Hand *hand, int origin, int dest, int armies, int targetPlayer) {
    deck->addCard(this);
    hand->removeCard(this);
    return new BombOrder(origin);
}

Card *BombCard::clone() {
    return new BombCard();
}

ostream &BombCard::print(ostream &out) const {
    out << "BombCard{}";
    return out;
}

BombCard::~BombCard() {}



//=============================
// ReinforcementCard Implementation
//=============================

ReinforcementCard::ReinforcementCard() {}

ReinforcementCard::ReinforcementCard(const ReinforcementCard &other) {}

void swap(ReinforcementCard &a, ReinforcementCard &b) {
    using std::swap;
}

ReinforcementCard &ReinforcementCard::operator=(ReinforcementCard other) {
    swap(*this, other);
    return *this;
}

ostream &operator<<(ostream &out, const ReinforcementCard &obj) {
    obj.print(out);
    return out;
}

Order *ReinforcementCard::play(Deck *deck, Hand *hand, int origin, int dest, int armies, int targetPlayer) {
    deck->addCard(this);
    hand->removeCard(this);
    return new DeployOrder(5, origin);
}

Card *ReinforcementCard::clone() {
    return new ReinforcementCard();
}

ostream &ReinforcementCard::print(ostream &out) const {
    out << "ReinforcementCard{}";
    return out;
}

ReinforcementCard::~ReinforcementCard() {}



//=============================
// BlockadeCard Implementation
//=============================

BlockadeCard::BlockadeCard() {}

BlockadeCard::BlockadeCard(const BlockadeCard &other) {}

void swap(BlockadeCard &a, BlockadeCard &b) {
    using std::swap;
}

BlockadeCard &BlockadeCard::operator=(BlockadeCard other) {
    swap(*this, other);
    return *this;
}

ostream &operator<<(ostream &out, const BlockadeCard &obj) {
    obj.print(out);
    return out;
}

Order *BlockadeCard::play(Deck *deck, Hand *hand, int origin, int dest, int armies, int targetPlayer) {
    deck->addCard(this);
    hand->removeCard(this);
    return new BlockadeOrder(origin);
}

Card *BlockadeCard::clone() {
    return new BlockadeCard();
}

ostream &BlockadeCard::print(ostream &out) const {
    out << "BlockadeCard{}";
    return out;
}

BlockadeCard::~BlockadeCard() {}



//=============================
// AirliftCard Implementation
//=============================

AirliftCard::AirliftCard() {}

AirliftCard::AirliftCard(const AirliftCard &other) {}

void swap(AirliftCard &a, AirliftCard &b) {
    using std::swap;
}

AirliftCard &AirliftCard::operator=(AirliftCard other) {
    swap(*this, other);
    return *this;
}

ostream &operator<<(ostream &out, const AirliftCard &obj) {
    obj.print(out);
    return out;
}

Order *AirliftCard::play(Deck *deck, Hand *hand, int origin, int dest, int armies, int targetPlayer) {
    deck->addCard(this);
    hand->removeCard(this);
    return new AirliftOrder(armies, origin, dest);
}

Card *AirliftCard::clone() {
    return new AirliftCard();
}

ostream &AirliftCard::print(ostream &out) const {
    out << "AirliftCard{}";
    return out;
}

AirliftCard::~AirliftCard() {}



//=============================
// DiplomacyCard Implementation
//=============================

DiplomacyCard::DiplomacyCard() {}

DiplomacyCard::DiplomacyCard(const DiplomacyCard &other) {}

void swap(DiplomacyCard &a, DiplomacyCard &b) {
    using std::swap;
}

DiplomacyCard &DiplomacyCard::operator=(DiplomacyCard other) {
    swap(*this, other);
    return *this;
}

ostream &operator<<(ostream &out, const DiplomacyCard &obj) {
    obj.print(out);
    return out;
}

Order *DiplomacyCard::play(Deck *deck, Hand *hand, int origin, int dest, int armies, int targetPlayer) {
    deck->addCard(this);
    hand->removeCard(this);
    return new NegotiateOrder(targetPlayer);
}

Card *DiplomacyCard::clone() {
    return new DiplomacyCard();
}

ostream &DiplomacyCard::print(ostream &out) const {
    out << "DiplomacyCard{}";
    return out;
}


DiplomacyCard::~DiplomacyCard() {}

