#include "Cards.h"
#include "../orders/Orders.h"
#include "../player/Player.h"


using std::endl;
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
    for (const auto &card : obj.cards) {
        out << card << endl;
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

Deck::~Deck() {}

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
    for (const auto &card : obj.cards) {
        out << card << endl;
    }
    out << "]" << endl << "}" << endl;

    return out;
}

void Hand::addCard(Card *card) {
    cards.push_back(card);
}

Card *Hand::getCard(int i) {
    return cards[i];
}


Hand::~Hand() {}


//=============================
// Card Implementation
//=============================

Card::Card() {}

Card::Card(const Card &other) {}

void swap(Card &a, Card &b) {
    using std::swap;
}

ostream &operator<<(ostream &out, const Card &obj) {
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
    out << "BombCard{}";
    return out;
}

Order *BombCard::play(int origin, int dest, int armies, int targetPlayer) {
    return new BombOrder(origin);
}

Card *BombCard::clone() {
    return new BombCard();
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
    out << "ReinforcementCard{}";
    return out;
}

Order *ReinforcementCard::play(int origin, int dest, int armies, int targetPlayer) {

    return nullptr;
}

Card *ReinforcementCard::clone() {
    return new ReinforcementCard();
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
    out << "BlockadeCard{}";
    return out;
}

Order *BlockadeCard::play(int origin, int dest, int armies, int targetPlayer) {
    return new BlockadeOrder(origin);
}

Card *BlockadeCard::clone() {
    return new BlockadeCard();
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
    out << "AirliftCard{}";
    return out;
}

Order *AirliftCard::play(int origin, int dest, int armies, int targetPlayer) {
    return new AirliftOrder(armies, origin, dest);
}

Card *AirliftCard::clone() {
    return new AirliftCard();
}

BlockadeCard::~AirliftCard() {}

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
    out << "DiplomacyCard{}";
    return out;
}

Order *DiplomacyCard::play(int origin, int dest, int armies, int targetPlayer) {
    return new NegotiateOrder(targetPlayer);
}

Card *DiplomacyCard::clone() {
    return new DiplomacyCard();
}

BlockadeCard::~DiplomacyCard() {}