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
          ownedTerritories{set<Territory *>()},
          allies{set<Player *>()},
          hand{new Hand()},
          orders{new OrdersList()} {}

Player::Player(const Player &other)
        : name{other.name},
          armies{other.armies},
          ownedTerritories{set<Territory *>(other.ownedTerritories)},
          allies{set<Player *>(other.allies)},
          hand{new Hand(*other.hand)},
          orders{new OrdersList()} {}

/**
 * Swap method for copy-and-swap
 * @param a first element
 * @param b second element
 */
void swap(Player &a, Player &b) {
    using std::swap;

    swap(a.name, b.name);
    swap(a.armies, b.armies);
    swap(a.ownedTerritories, b.ownedTerritories);
    swap(a.allies, b.allies);
    swap(a.hand, b.hand);
    swap(a.orders, b.orders);
}

Player &Player::operator=(Player other) {
    swap(*this, other);
    return *this;
}

ostream &operator<<(ostream &out, const Player &obj) {
    out << "Player{" << endl
        << "name: \"" << obj.name << "\"" << endl
        << "ownedTerritories[" << obj.ownedTerritories.size() << "][ ";
    for (auto territory : obj.ownedTerritories) {
        out << territory->getName() << " ";
    }
    out << "] " << endl
        << "allies[" << obj.allies.size() << "][ ";
    for (auto ally : obj.allies) {
        out << ally->getName() << " ";
    }
    out << "] " << endl
        << "hand: " << *obj.hand << endl
        << "orders: " << *obj.orders << endl
        << "}" << endl;
    return out;
}

/**
 * Returns list of territories to defend
 */
vector<Territory *> Player::toDefend() {
    vector<Territory *> toDefend{};
    vector<Territory *> availableTerritories = setToVector(ownedTerritories);

    pickTerritoriesFromList(availableTerritories, toDefend,
                            "Among these territories which you own:", "Which do you want to defend most?");


    return toDefend;
}

/**
 * Returns list of territories to attack
 */
vector<Territory *> Player::toAttack(Map *map) {
    vector<Territory *> toAttack{};
    vector<Territory *> availableTerritories = getNeighboringTerritories(map);
    pickTerritoriesFromList(availableTerritories, toAttack,
                            "Among these neighboring enemy territories:", "Which do you want to attack most?");
    return toAttack;
}

void Player::issueOrder(Map *map, Deck *deck) {
    cout << name << " is issuing orders!" << endl << endl;

    vector<Territory *> attack = toAttack(map);
    vector<Territory *> defend = toDefend();

    // Deploy orders
    while (armies > 0) {
        Territory *deployTo = pickFromList(
                name + " has " + to_string(armies) + " armies. They will be prompted for deploy orders.",
                "Which territory do you want to deploy to?", defend);

        int armiesToDeploy = getIntInput("How many armies do you want to deploy?", 1, armies);
        issueDeployOrder(armiesToDeploy, deployTo);
        armies -= armiesToDeploy;
    }

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
            cout << "Here are the territories you set to defend:" << endl;
            for (int i = 0; i < defend.size(); ++i) {
                cout << "\t" << (i + 1) << ": " << defend[i]->getName() << endl;
            }
            dest = defend[getIntInput("Which territory to defend?", 1, defend.size()) - 1];

            vector<Territory *> senders{};
            for (auto &neighbor : map->getNeighbors(dest)) {
                if (neighbor->getPlayer() == this && neighbor->getArmies() > 0) {
                    senders.push_back(neighbor);
                }
            }
            cout << "Here are territories you own which neighbor the territory you want to defend:" << endl;
            for (int i = 0; i < senders.size(); ++i) {
                cout << "\t" << (i + 1) << ": " << senders[i]->getName() << ", "
                     << senders[i]->getArmies() << " armies" << endl;
            }
            origin = senders[getIntInput("Which of your territories should send armies?",
                                         1, senders.size()) - 1];
        }
        armies = getIntInput("How many armies should be sent?", 1, origin->getArmies());
        issueAdvanceOrder(armies, origin, dest);
    } while (true);

    if (!hand->empty()) {
        cout << name << " has the following cards: " << endl;
        for (int i = 0; i < (*hand).size(); ++i) {
            cout << "\t" << (i + 1) << ": " << (*hand)[i] << endl;
        }
        bool play = getBoolInput("Do you want to play a card?");
        if (play) {
            Territory *origin = nullptr;
            Territory *dest = nullptr;
            Player *targetPlayer = nullptr;
            int armies = 0;

            int cardIndex = getIntInput("Pick a card to play", 1, hand->getCards().size()) - 1;
            hand->getCard(cardIndex)->play(nullptr, deck, nullptr);
        }
    }


}

/**
 * Issue DeployOrder
 * @param armies
 * @param territory
 */
void Player::issueDeployOrder(int armies, Territory *territory) {
    DeployOrder *order = new DeployOrder(armies, territory);
    cout << "Created order:" << *order << endl;
    orders->add(order);
}

/**
 * Issue AdvanceOrder
 * @param armies
 * @param territory
 */
void Player::issueAdvanceOrder(int armies, Territory *origin, Territory *dest) {
    AdvanceOrder *order = new AdvanceOrder(armies, origin, dest);
    cout << "Created order:" << *order << endl;
    orders->add(order);
}

/**
 * Issue Order from a Card
 * @param armies
 * @param territory
 */
void Player::issueOrder(Deck *deck, Card *card, int armies, Territory *origin, Territory *dest, Player *targetPlayer) {
    orders->add(card->play(nullptr, deck, nullptr));
}

void Player::captureTerritory(Territory *territory) {
    // If you capture a territory remove from opposing player's owned territories
    ownedTerritories.insert(territory);
    if (territory->getPlayer() != nullptr) {
        territory->getPlayer()->loseTerritory(territory);
    }
    territory->setPlayer(this);
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

void Player::addCardOrder(Order *order) {
    orders->add(order);
}

const string &Player::getName() const {
    return name;
}

const int &Player::getArmies() const {
    return armies;
}

OrdersList &Player::getOrders() {
    return *orders;
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

void Player::addArmies(int armies) {
    this->armies += armies;
}

void Player::removeArmies(int armies) {
    if (this->armies < armies) {
        throw "Player" + this->getName() + " doesn't have enough armies: " + to_string(armies);
    }
}

Player::~Player() {
    delete hand;
    hand = nullptr;

    delete orders;
    orders = nullptr;
}



























