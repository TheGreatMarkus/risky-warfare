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

Deck::Deck() : cards{} {}

Deck::Deck(const Deck &other)
        : cards{vector<Card *>{}} {
    for (const auto &card : other.cards) {
        cards.push_back(card->clone());
    }
}

/**
 * Swap method. Used for the copy-and-swap idiom
 *
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
    out << "Deck{ ";
    for (auto card : obj.cards) {
        out << *card << ", ";
    }
    out << "}" << endl;

    return out;
}

/**
 * Picks a random cards and puts it to the given hand.
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

Hand::Hand(const Hand &other) : cards{vector<Card *>()} {
    for (const auto &card : other.cards) {
        cards.push_back(card->clone());
    }
}

/**
 * Swap method. Used for the copy-and-swap idiom
 *
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
    out << "Hand{ ";
    for (auto card : obj.cards) {
        out << *card << ", ";
    }
    out << "}" << endl;

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

ostream &operator<<(ostream &out, const Card &obj) {
    obj.print(out);
    return out;
}

Card::~Card() {}

//=============================
// BombCard Implementation
//=============================


/**
 * Plays the current BombCard.
 *
 * Playing a card first moves the card from the player's hand to the deck.
 * Then, a new BombOrder is returned.
 *
 * @param cardPlayer
 * @param deck
 * @param map
 * @param players
 * @return A new BombOrder
 */
Order *BombCard::play(Player *cardPlayer, Deck *deck, Map *map, vector<Player *> players) {

    deck->addCard(this);
    cardPlayer->getHand()->removeCard(this);

    vector<Territory *> neighbors = cardPlayer->getNeighboringTerritories(map);
    Territory *origin = pickFromList("Among these enemy neighbors:", "Which should be bombed?", neighbors);

    return new BombOrder(origin);
}

/**
 * Helper function for polymorphic cloning
 */
Card *BombCard::clone() {
    return new BombCard(*this);
}

/**
 * Helper print function for polymorphic stream insertion
 *
 * @param out
 */
void BombCard::print(ostream &out) const {
    out << "BombCard";
}



//=============================
// ReinforcementCard Implementation
//=============================

/**
 * Plays the current ReinforcementCard.
 *
 * Playing a card first moves the card from the player's hand to the deck.
 * Then, a new DeployOrder is returned.
 *
 * @param cardPlayer
 * @param deck
 * @param map
 * @param players
 * @return A new DeployOrder
 */
Order *ReinforcementCard::play(Player *cardPlayer, Deck *deck, Map *map, vector<Player *> players) {
    deck->addCard(this);
    cardPlayer->getHand()->removeCard(this);

    vector<Territory *> ownedTerritories = setToVector(cardPlayer->getOwnedTerritories());
    Territory *target = pickFromList("Among these territories you own:",
                                     "Which should be reinforced?",
                                     ownedTerritories);

    target->addArmies(5);

    return new DeployOrder(5, target);
}

/**
 * Helper function for polymorphic cloning
 */
Card *ReinforcementCard::clone() {
    return new ReinforcementCard(*this);
}

/**
 * Helper print function for polymorphic stream insertion
 *
 * @param out
 */
void ReinforcementCard::print(ostream &out) const {
    out << "ReinforcementCard";
}

//=============================
// BlockadeCard Implementation
//=============================

/**
 * Plays the current BlockadeCard.
 *
 * Playing a card first moves the card from the player's hand to the deck.
 * Then, a new BlockadeOrder is returned.
 *
 * @param cardPlayer
 * @param deck
 * @param map
 * @param players
 * @return A new BlockadeOrder
 */
Order *BlockadeCard::play(Player *cardPlayer, Deck *deck, Map *map, vector<Player *> players) {
    deck->addCard(this);
    cardPlayer->getHand()->removeCard(this);

    vector<Territory *> ownedTerritories = setToVector(cardPlayer->getOwnedTerritories());
    Territory *target = pickFromList("Among these territories you own:",
                                     "Which should be blockaded?",
                                     ownedTerritories);

    return new BlockadeOrder(target);
}

/**
 * Helper function for polymorphic cloning
 */
Card *BlockadeCard::clone() {
    return new BlockadeCard(*this);
}

/**
 * Helper print function for polymorphic stream insertion
 *
 * @param out
 */
void BlockadeCard::print(ostream &out) const {
    out << "BlockadeCard";
}

//=============================
// AirliftCard Implementation
//=============================

/**
 * Plays the current AirliftCard.
 *
 * Playing a card first moves the card from the player's hand to the deck.
 * Then, a new AirliftOrder is returned.
 *
 * @param cardPlayer
 * @param deck
 * @param map
 * @param players
 * @return A new AirliftOrder
 */
Order *AirliftCard::play(Player *cardPlayer, Deck *deck, Map *map, vector<Player *> players) {
    deck->addCard(this);
    cardPlayer->getHand()->removeCard(this);

    vector<Territory *> validOrigins{};
    for (auto &territory : cardPlayer->getOwnedTerritories()) {
        if (territory->getAvailableArmies() > 0) {
            validOrigins.push_back(territory);
        }
    }

    Territory *origin = pickFromList("From the territories you own that have armies:",
                                     "Which territory do you want to airlift from?",
                                     validOrigins);
    vector<Territory *> validDestinations = vector(map->getTerritories().begin(), map->getTerritories().end());
    removeElement(validDestinations, origin);
    Territory *dest = pickFromList("From all territories on the map:", "Which territory do you want to airlift to?",
                                   validDestinations);

    int armies = getIntInput("How many armies do you want to send?", 1, origin->getAvailableArmies());

    origin->reserveArmies(armies);
    return new AirliftOrder(armies, origin, dest);
}

/**
 * Helper function for polymorphic cloning
 */
Card *AirliftCard::clone() {
    return new AirliftCard(*this);
}

/**
 * Helper print function for polymorphic stream insertion
 *
 * @param out
 */
void AirliftCard::print(ostream &out) const {
    out << "AirliftCard";
}

//=============================
// DiplomacyCard Implementation
//=============================

/**
 * Plays the current DiplomacyCard.
 *
 * Playing a card first moves the card from the player's hand to the deck.
 * Then, a new NegotiateOrder is returned.
 *
 * @param cardPlayer
 * @param deck
 * @param map
 * @param players
 * @return A new NegotiateOrder
 */
Order *DiplomacyCard::play(Player *cardPlayer, Deck *deck, Map *map, vector<Player *> activePlayers) {
    deck->addCard(this);
    cardPlayer->getHand()->removeCard(this);

    removeElement(activePlayers, cardPlayer);

    Player *targetPlayer = pickFromList("Among the players still in the game:", "Who do you want to negotiate with?",
                                        activePlayers);

    return new NegotiateOrder(targetPlayer);
}

/**
 * Helper function for polymorphic cloning
 */
Card *DiplomacyCard::clone() {
    return new DiplomacyCard(*this);
}

/**
 * Helper print function for polymorphic stream insertion
 *
 * @param out
 */
void DiplomacyCard::print(ostream &out) const {
    out << "DiplomacyCard";
}
