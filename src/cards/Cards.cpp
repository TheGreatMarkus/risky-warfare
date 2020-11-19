#include "Cards.h"

#include <algorithm>

#include "../orders/Orders.h"
#include "../utils/Utils.h"
#include "../player/Player.h"

using std::endl;
using std::remove;
using std::cout;

using cris_utils::randInt;
using cris_utils::getIntInput;
using cris_utils::setToVector;
using cris_utils::pickFromList;
using cris_utils::removeElement;

//=============================
// Deck Implementation
//=============================

Deck::Deck()
        : cards{} {}

/**
 * Copy constructor for Deck.
 *
 * Creates copies of all cards and puts them in a new list.
 * @param other
 */
Deck::Deck(const Deck &other)
        : cards{vector<Card *>{}} {
    for (const auto &card : other.cards) {
        cards.push_back(card->clone());
    }
}

/**
 * Swap method for copy-and-swap
 * @param a first element
 * @param b second element
 */
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

/**
 * Picks a random cards and moves it to the given hand.
 * @param hand
 */
void Deck::draw(Hand *hand) {
    int picked = randInt(0, cards.size() - 1);

    Card *pickedCard = cards[picked];
    removeElement(cards, pickedCard);

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

/**
 * Copy constructor for Hand.
 *
 * Creates copies of all cards and puts them in a new list.
 * @param other
 */
Hand::Hand(const Hand &other) : cards{vector<Card *>()} {
    for (const auto &card : other.cards) {
        cards.push_back(card->clone());
    }
}

/**
 * Swap method for copy-and-swap
 * @param a first element
 * @param b second element
 */
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

Card *Hand::operator[](int i) {
    return cards[i];
}

const int Hand::size() const {
    return cards.size();
}

const bool Hand::empty() const {
    return cards.empty();
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

const vector<Card *> &Hand::getCards() const {
    return cards;
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

/**
 * Swap method for copy-and-swap
 * @param a first element
 * @param b second element
 */
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

/**
 * Swap method for copy-and-swap
 * @param a first element
 * @param b second element
 */
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

/**
 * Plays the current BombCard by first moving the card from the hand to the deck, and creating a new BombOrder.
 * @param deck
 * @param hand
 * @param origin
 * @param dest
 * @param armies
 * @param targetPlayer
 * @return a new BombOrder
 */
Order *BombCard::play(Player *cardPlayer, Deck *deck, Map *map, vector<Player *> players) {

    deck->addCard(this);
    cardPlayer->getHand()->removeCard(this);

    vector<Territory *> neighbors = cardPlayer->getNeighboringTerritories(map);
    Territory *origin = pickFromList("Among these enemy neighbors:", "Which should be bombed?", neighbors);

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

/**
 * Swap method for copy-and-swap
 * @param a first element
 * @param b second element
 */
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

/**
 * Plays the current ReinforcementCard by first moving the card from the hand to the deck, and creating a new DeployOrder.
 * @param deck
 * @param hand
 * @param origin
 * @param dest
 * @param armies
 * @param targetPlayer
 * @return a new BombOrder
 */
Order *
ReinforcementCard::play(Player *cardPlayer, Deck *deck, Map *map, vector<Player *> players) {
    deck->addCard(this);
    cardPlayer->getHand()->removeCard(this);

    vector<Territory *> ownedTerritories = setToVector(cardPlayer->getOwnedTerritories());
    Territory *target = pickFromList("Among these territories you own:", "Which should be reinforced?",
                                     ownedTerritories);

    return new DeployOrder(5, target);
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

/**
 * Swap method for copy-and-swap
 * @param a first element
 * @param b second element
 */
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

/**
 * Plays the current BlockadeCard by first moving the card from the hand to the deck, and creating a new BlockadeOrder.
 * @param deck
 * @param hand
 * @param origin
 * @param dest
 * @param armies
 * @param targetPlayer
 * @return a new BombOrder
 */
Order *
BlockadeCard::play(Player *cardPlayer, Deck *deck, Map *map, vector<Player *> players) {
    deck->addCard(this);
    cardPlayer->getHand()->removeCard(this);

    vector<Territory *> ownedTerritories = setToVector(cardPlayer->getOwnedTerritories());
    Territory *target = pickFromList("Among these territories you own:", "Which should be blockaded?",
                                     ownedTerritories);

    return new BlockadeOrder(target);
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

/**
 * Swap method for copy-and-swap
 * @param a first element
 * @param b second element
 */
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

/**
 * Plays the current AirliftCard by first moving the card from the hand to the deck, and creating a new AirliftOrder.
 * @param deck
 * @param hand
 * @param origin
 * @param dest
 * @param armies
 * @param targetPlayer
 * @return a new BombOrder
 */
Order *AirliftCard::play(Player *cardPlayer, Deck *deck, Map *map, vector<Player *> players) {
    deck->addCard(this);
    cardPlayer->getHand()->removeCard(this);

    vector<Territory *> validOrigins{};
    for (auto &territory : cardPlayer->getOwnedTerritories()) {
        if (territory->getArmies() > 0) {
            validOrigins.push_back(territory);
        }
    }

    Territory *origin = pickFromList("From the territories you own that have armies:",
                                     "Which territory do you want to airlift from?",
                                     validOrigins);

    Territory *dest = pickFromList("From all territories on the map:", "Which territory do you want to airlift to?",
                                   map->getTerritories());

    int armies = getIntInput("How many armies do you want to send?", 1, origin->getArmies());

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

/**
 * Swap method for copy-and-swap
 * @param a first element
 * @param b second element
 */
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

/**
 * Plays the current DiplomacyCard by first moving the card from the hand to the deck, and creating a new AirliftOrder.
 * @param deck
 * @param hand
 * @param origin
 * @param dest
 * @param armies
 * @param targetPlayer
 * @return a new NegotiateOrder
 */
Order *
DiplomacyCard::play(Player *cardPlayer, Deck *deck, Map *map, vector<Player *> activePlayers) {
    deck->addCard(this);
    cardPlayer->getHand()->removeCard(this);

    removeElement(activePlayers, cardPlayer);

    Player *targetPlayer = pickFromList("Among the players still in the game:", "Who do you want to negotiate with?",
                                        activePlayers);

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

