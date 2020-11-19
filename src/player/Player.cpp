#include "Player.h"

#include "../cards/Cards.h"
#include "../orders/Orders.h"
#include "../utils/Utils.h"
#include "../map/Map.h"

using std::endl;
using std::cout;
using std::to_string;

using cris_utils::contains;
using cris_utils::removeElement;
using cris_utils::getIntInput;
using cris_utils::getBoolInput;
using cris_utils::setToVector;
using cris_utils::pickFromList;


namespace {
    void pickTerritoriesFromList(vector<Territory *> &available, vector<Territory *> &toFill,
                                 string desc, string prompt) {
        bool stop;
        do {
            Territory *choice = pickFromList(desc, prompt, available);

            toFill.push_back(choice);
            removeElement(available, choice);

            if (available.empty()) {
                stop = true;
            }

            if (!stop) {
                stop = getBoolInput("Do you want to stop picking territories?");
            }

        } while (!stop);
    }
}


Player::Player(string name)
        : name{name},
          armies{0},
          cardDue{false},
          ownedTerritories{set<Territory *>()},
          allies{set<Player *>()},
          hand{new Hand()},
          ordersList{new OrdersList()} {}

Player::Player(const Player &other)
        : name{other.name},
          armies{other.armies},
          cardDue{other.cardDue},
          ownedTerritories{set<Territory *>(other.ownedTerritories)},
          allies{set<Player *>(other.allies)},
          hand{new Hand(*other.hand)},
          ordersList{new OrdersList()} {}

/**
 * Swap method for copy-and-swap
 * @param a first element
 * @param b second element
 */
void swap(Player &a, Player &b) {
    using std::swap;

    swap(a.name, b.name);
    swap(a.armies, b.armies);
    swap(a.cardDue, b.cardDue);
    swap(a.ownedTerritories, b.ownedTerritories);
    swap(a.allies, b.allies);
    swap(a.hand, b.hand);
    swap(a.ordersList, b.ordersList);
}

Player &Player::operator=(Player other) {
    swap(*this, other);
    return *this;
}

ostream &operator<<(ostream &out, const Player &obj) {
    out << "Player{ name: \"" << obj.name << "\""
        << ", armies: " << obj.armies
        << ", cardDue: " << obj.cardDue
        << ", ownedTerritories[" << obj.ownedTerritories.size() << "]"
        << ", allies[" << obj.allies.size() << "]"
        << ", hand[" << obj.hand->size() << "]"
        << ", orders[" << obj.ordersList->size() << "]"
        << " }";
    return out;
}

/**
 * Returns list of territories to defend
 */
vector<Territory *> Player::toDefend(Map *map) {
    vector<Territory *> available = setToVector(ownedTerritories);
    vector<Territory *> toDefend{};
    vector<Territory *> canDefend{};

    // Filter for enemy territories which can be attacked
    for (auto &ownedTerritory : ownedTerritories) {
        for (const auto &neighbor : map->getNeighbors(ownedTerritory)) {
            if (neighbor->getPlayer() == this && neighbor->getArmies() > 0) {
                canDefend.push_back(ownedTerritory);
            }
        }
    }
    if (!canDefend.empty()) {
        pickTerritoriesFromList(canDefend, toDefend,
                                "You can defend the following territories:", "Which do you want to defend most?");
    }


    return toDefend;
}

/**
 * Returns list of territories to attack
 */
vector<Territory *> Player::toAttack(Map *map) {
    vector<Territory *> enemyTerritories = getNeighboringTerritories(map);
    vector<Territory *> toAttack{};
    vector<Territory *> canAttack{};

    // Filter for enemy territories which can be attacked
    for (auto &enemyTerritory : enemyTerritories) {
        for (const auto &neighbor : map->getNeighbors(enemyTerritory)) {
            if (neighbor->getPlayer() == this && neighbor->getArmies() > 0) {
                canAttack.push_back(enemyTerritory);
            }
        }
    }
    if (!canAttack.empty()) {
        pickTerritoriesFromList(canAttack, toAttack,
                                "You can attack the following territories:", "Which do you want to attack most?");
    }
    return toAttack;
}

void Player::issueOrder(Map *map, Deck *deck, vector<Player *> activePlayers) {
    cout << name << " is issuing orders!" << endl << endl;


    // Deploy orders
    while (armies > 0) {
        cout << endl;
        Territory *deployTo = pickFromList(
                name + " has " + to_string(armies) + " armies.",
                "Which territory do you want to deploy to?", setToVector(ownedTerritories));

        int armiesToDeploy = getIntInput("How many armies do you want to deploy?", 1, armies);
        issueDeployOrder(armiesToDeploy, deployTo, map);
    }

    vector<Territory *> attack = toAttack(map);
    vector<Territory *> defend = toDefend(map);

    bool stop = false;
    do {
        cout << endl;
        stop = !getBoolInput("Do you want to issue an advance order?");
        if (stop) {
            break;
        }
        Territory *origin;
        Territory *dest;
        int armies = 0;
        int action = getIntInput("Do you want to attack (1) or defend (2)?", 1, 2);
        if (action == 1) {
            if (attack.empty()) {
                cout << "you cannot attack" << endl;
                continue;
            }
            dest = pickFromList("Here are the territories you set to attack:",
                                "Which territory do you want to attack?", attack);
            vector<Territory *> attackers{};
            for (auto &neighbor : map->getNeighbors(dest)) {
                if (neighbor->getPlayer() == this && neighbor->getArmies() > 0) {
                    attackers.push_back(neighbor);
                }
            }
            origin = pickFromList("Here are territories you own which neighbor the territory you want to attack:",
                                  "Which of your territories should perform the attack?", attackers);

        } else {
            if (defend.empty()) {
                cout << "you cannot defend" << endl;
                continue;
            }
            dest = pickFromList("Here are the territories you set to defend:",
                                "Which territory do you want to defend?", defend);

            vector<Territory *> senders{};
            for (auto &neighbor : map->getNeighbors(dest)) {
                if (neighbor->getPlayer() == this && neighbor->getArmies() > 0) {
                    senders.push_back(neighbor);
                }
            }
            origin = pickFromList("Here are territories you own which neighbor the territory you want to defend:",
                                  "Which of your territories should send armies?", senders);
        }
        armies = getIntInput("How many armies should be sent?", 1, origin->getArmies());
        issueAdvanceOrder(armies, origin, dest);
    } while (true);

    if (!hand->empty()) {
        bool play = getBoolInput("Do you want to play a card?");
        if (play) {
            Card *cardToPlay = pickFromList(name + " has the following cards:", "Pick a card to play",
                                            hand->getCards());
            cout << "Playing " << *cardToPlay << endl;
            Order *cardOrder = cardToPlay->play(this, deck, map, activePlayers);
            cout << name << " issued " << *cardOrder << endl;
            ordersList->add(cardOrder);
        }
    }


}

/**
 * Issue DeployOrder
 * @param armies
 * @param territory
 */
void Player::issueDeployOrder(int armies, Territory *territory, Map *map) {
    DeployOrder *order = new DeployOrder(armies, territory);
    cout << name << " issued " << *order << endl;
    cout << "Executing " << *order << endl;
    order->execute(map, this);
    ordersList->add(order);
}

/**
 * Issue AdvanceOrder
 * @param armies
 * @param territory
 */
void Player::issueAdvanceOrder(int armies, Territory *origin, Territory *dest) {
    AdvanceOrder *order = new AdvanceOrder(armies, origin, dest);
    cout << name << " issued " << *order << endl;
    ordersList->add(order);
}


void Player::captureTerritory(Territory *territory) {
    // If territory belonged to a player, remove from their ownedTerritories
    ownedTerritories.insert(territory);
    if (territory->getPlayer() != nullptr) {
        territory->getPlayer()->loseTerritory(territory);
    }
    territory->setPlayer(this);
    cardDue = true;
}

void Player::loseTerritory(Territory *territory) {
    removeElement(ownedTerritories, territory);
    territory->setPlayer(nullptr);
}

/**
 * Checks whether player own a territory
 * @param territory
 * @return
 */
bool Player::owns(Territory *territory) {
    return contains(ownedTerritories, territory);
}

/**
 * Add ally after negotiation
 * @param otherPlayer
 */
void Player::addAlly(Player *otherPlayer) {
    allies.insert(otherPlayer);
}

void Player::resetAllies() {
    allies.clear();
}


const string &Player::getName() const {
    return name;
}

const int &Player::getArmies() const {
    return armies;
}

bool Player::isCardDue() const {
    return cardDue;
}

const set<Territory *> &Player::getOwnedTerritories() const {
    return ownedTerritories;
}

const vector<Territory *> Player::getNeighboringTerritories(Map *map) const {
    vector<Territory *> neighbors{};
    for (auto &territory : map->getTerritories()) {
        if (territory->getPlayer() != this) {
            for (auto &ownedTerritory : ownedTerritories) {
                if (map->areAdjacent(territory, ownedTerritory)) {
                    neighbors.push_back(territory);
                    break;
                }
            }
        }
    }
    return neighbors;
}

Hand *Player::getHand() {
    return hand;
}

OrdersList *Player::getOrdersList() const {
    return ordersList;
}

void Player::addArmies(int armies) {
    this->armies += armies;
}

void Player::removeArmies(int armies) {
    this->armies -= armies;
    if (this->armies < 0) {
        this->armies = 0;
    }
}

void Player::setCardDue(bool cardDue) {
    Player::cardDue = cardDue;
}

Player::~Player() {
    delete hand;
    hand = nullptr;

    delete ordersList;
    ordersList = nullptr;
}

const set<Player *> &Player::getAllies() const {
    return allies;
}





