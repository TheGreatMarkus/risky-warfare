#include "PlayerStrategies.h"

#include <set>
#include <vector>
#include <algorithm>

#include "../utils/Utils.h"
#include "../player/Player.h"
#include "../cards/Cards.h"
#include "../orders/Orders.h"

using std::set;
using std::vector;
using std::sort;

using cris_utils::setToVector;
using cris_utils::pickFromList;
using cris_utils::removeElement;
using cris_utils::getBoolInput;
using cris_utils::getIntInput;
using cris_utils::getContinueInput;

namespace {
    void pickTerritoriesFromList(vector<Territory *> &available, vector<Territory *> &toFill,
                                 string desc, string prompt) {
        bool stop = false;
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

    bool lowToHigh(Territory *t0, Territory *t1) {
        return t0->getArmies() < t1->getArmies();
    }

    bool highToLow(Territory *t0, Territory *t1) {
        return t0->getArmies() > t1->getArmies();
    }

    vector<Territory *> sortTerritoriesByArmies(vector<Territory *> territories, bool reverse = false) {
        if (reverse) {
            sort(territories.begin(), territories.end(), highToLow);
        } else {
            sort(territories.begin(), territories.end(), lowToHigh);
        }

        return territories;
    }
}


//=============================
// PlayerStrategy Implementation
//=============================

PlayerStrategy::PlayerStrategy(Player *player) : player{player} {}

ostream &operator<<(ostream &out, const PlayerStrategy &obj) {
    obj.print(out);
    return out;
}

// PlayerStrategy isn't responsible for managing Player pointers
PlayerStrategy::~PlayerStrategy() {}

//=============================
// HumanPlayerStrategy Implementation
//=============================

const string ADVANCE_ATTACK_OPTION = "Issue an Advance order to attack";
const string ADVANCE_DEFEND_OPTION = "Issue an Advance order to defend";
const string PLAY_CARD_OPTION = "Play a card";
const string SKIP_OPTION = "Skip my turn";

void HumanPlayerStrategy::fillInAdvanceOrder(Map *map, Territory *&origin, Territory *&dest, int &armies, string verb,
                                             vector<Territory *> list) {
    dest = pickFromList("Here are the territories you set to " + verb + ":",
                        "Pick one to " + verb + "", list);
    vector<Territory *> origins{};
    for (auto &neighbor : map->getNeighbors(dest)) {
        if (neighbor->getPlayer() == player && neighbor->getAvailableArmies() > 0) {
            origins.push_back(neighbor);
        }
    }
    origin = pickFromList(
            "Here are your territories with available armies which neighbor the territory you want to " + verb + ":",
            "Pick the " + verb + "ing territory", origins);

    armies = getIntInput("How many armies should be sent?", 1, origin->getAvailableArmies());
}

HumanPlayerStrategy::HumanPlayerStrategy(Player *player) : PlayerStrategy(player) {}

bool HumanPlayerStrategy::issueOrder(Map *map, Deck *deck, vector<Player *> activePlayers) {
    cout << "Employing [HumanPlayerStrategy] to issue orders" << endl;
    // Deploy orders
    if (player->getArmies() > 0) {
        Territory *territory = pickFromList(
                player->getName() + ", here are your territories:",
                "Which territory do you want to deploy to?", setToVector(player->getOwnedTerritories()));
        int armies = getIntInput("How many armies do you want to deploy?", 1, player->getArmies());
        player->issueDeployOrder(territory, armies);
        return false;
    }

    vector<Territory *> attack = toAttack(map);
    vector<Territory *> defend = toDefend(map);

    vector<string> options;

    if (!attack.empty()) {
        options.push_back(ADVANCE_ATTACK_OPTION);
    }
    if (!defend.empty()) {
        options.push_back(ADVANCE_DEFEND_OPTION);
    }
    if (!player->getHand()->empty()) {
        options.push_back(PLAY_CARD_OPTION);
    }
    if (!options.empty()) {
        options.push_back(SKIP_OPTION);
    } else {
        cout << "You have no possible moves right now" << endl;
        return true;
    }
    string answer = pickFromList("Given these possible options", "What do you want to do?", options);

    if (answer == ADVANCE_ATTACK_OPTION || answer == ADVANCE_DEFEND_OPTION) {
        Territory *origin = nullptr;
        Territory *dest = nullptr;
        int armies = 0;
        string verb = "";
        vector<Territory *> list;
        if (answer == ADVANCE_ATTACK_OPTION) {
            verb = "attack";
            list = attack;
        } else {
            verb = "defend";
            list = defend;
        }

        fillInAdvanceOrder(map, origin, dest, armies, verb, list);
        player->issueAdvanceOrder(origin, dest, armies);
    } else if (answer == PLAY_CARD_OPTION) {
        Card *cardToPlay = pickFromList(player->getName() + " has the following cards:",
                                        "Pick a card to play",
                                        player->getHand()->getCards());
        cout << "Playing " << *cardToPlay << endl;
        Order *cardOrder = cardToPlay->play(player, deck, map, activePlayers);
        cout << player->getHand() << " issued " << *cardOrder << endl;
        player->getOrders()->add(cardOrder);
    }
    return getBoolInput("Are you done issuing orders?");
}

vector<Territory *> HumanPlayerStrategy::toAttack(Map *map) {
    vector<Territory *> enemyTerritories = player->getNeighboringTerritories(map);
    vector<Territory *> toAttack{};
    vector<Territory *> canAttack{};

    // Filter for enemy territories which can be attacked
    for (auto &enemyTerritory : enemyTerritories) {
        for (const auto &neighbor : map->getNeighbors(enemyTerritory)) {
            if (neighbor->getPlayer() == player && neighbor->getAvailableArmies() > 0) {
                canAttack.push_back(enemyTerritory);
                break;
            }
        }
    }
    if (!canAttack.empty()) {
        pickTerritoriesFromList(canAttack, toAttack,
                                "You can attack the following territories:", "Which do you want to attack most?");
    }
    return toAttack;
}

vector<Territory *> HumanPlayerStrategy::toDefend(Map *map) {
    vector<Territory *> available = setToVector(player->getOwnedTerritories());
    vector<Territory *> toDefend{};
    vector<Territory *> canDefend{};

    // Filter for territories which can be defended
    for (auto &ownedTerritory : player->getOwnedTerritories()) {
        for (const auto &neighbor : map->getNeighbors(ownedTerritory)) {
            if (neighbor->getPlayer() == player && neighbor->getAvailableArmies() > 0) {
                canDefend.push_back(ownedTerritory);
                break;
            }
        }
    }
    if (!canDefend.empty()) {
        pickTerritoriesFromList(canDefend, toDefend,
                                "You can defend the following territories:", "Which do you want to defend most?");
    }

    return toDefend;
}

PlayerStrategy *HumanPlayerStrategy::clone() {
    return new HumanPlayerStrategy(*this);
}

ostream &HumanPlayerStrategy::print(ostream &out) const {
    out << "HumanPlayerStrategy";
    return out;
}

//=============================
// AggressivePlayerStrategy Implementation
//=============================
AggressivePlayerStrategy::AggressivePlayerStrategy(Player *player) : PlayerStrategy(player) {}

bool AggressivePlayerStrategy::issueOrder(Map *map, Deck *deck, vector<Player *> activePlayers) {
    cout << "Employing [AggressivePlayerStrategy] to issue orders" << endl;
    vector<Territory *> targets = toAttack(map);
    vector<Territory *> attackers = toDefend(map);
    Territory *attacker = attackers[0];

    if (player->getArmies() > 0) {
        // If player has armies, deploy them all to strongest territory that can attack
        player->issueDeployOrder(attacker, player->getArmies());
        getContinueInput();
        return false;
    }

    int availableArmies = attacker->getAvailableArmies();
    if (availableArmies > 0 && !targets.empty()) {
        Territory *target = targets[0];
        int armies = target->getArmies() * 2 + 1;
        if (armies > availableArmies) {
            armies = availableArmies;
        }
        player->issueAdvanceOrder(attacker, target, armies);
        getContinueInput();
        return false;
    }

    getContinueInput();
    return true;

}

vector<Territory *> AggressivePlayerStrategy::toAttack(Map *map) {
    Territory *attacker = toDefend(map)[0];
    vector<Territory *> targets;

    for (auto &neighbor :  map->getNeighbors(attacker)) {
        if (neighbor->getPlayer() != player) {
            // Check if neighbor has already been targeted
            bool alreadyTargeted = false;
            for (int i = 0; i < player->getOrders()->size(); ++i) {
                Order *order = (*player->getOrders())[i];
                if (order->getType() == OrderType::ADVANCE) {
                    AdvanceOrder *advanceOrder = dynamic_cast<AdvanceOrder *>(order);
                    if (advanceOrder->getDest() == neighbor) {
                        alreadyTargeted = true;
                    }
                }
            }
            // If we're already attacking neighbor, ignore
            if (!alreadyTargeted) {
                targets.push_back(neighbor);
            }
        }
    }

    return sortTerritoriesByArmies(targets);
}

vector<Territory *> AggressivePlayerStrategy::toDefend(Map *map) {
    vector<Territory *> sortedTerritories = sortTerritoriesByArmies(
            setToVector(player->getOwnedTerritories()), true);

    // Loop through territories from the strongest to weakest
    for (auto &territory : sortedTerritories) {
        // Check if territory can attack. If it can, then it will be reinforced
        if (territory->canAttack(map)) {
            return vector<Territory *>{territory};
        }
    }
    return vector<Territory *>();
}

PlayerStrategy *AggressivePlayerStrategy::clone() {
    return new AggressivePlayerStrategy(*this);
}

ostream &AggressivePlayerStrategy::print(ostream &out) const {
    out << "AggressivePlayerStrategy";
    return out;
}

//=============================
// BenevolentPlayerStrategy Implementation
//=============================

BenevolentPlayerStrategy::BenevolentPlayerStrategy(Player *player) : PlayerStrategy(player) {}

bool BenevolentPlayerStrategy::issueOrder(Map *map, Deck *deck, vector<Player *> activePlayers) {
    cout << "Employing [BenevolentPlayerStrategy] to issue orders" << endl;
    vector<Territory *> defend = toDefend(map);

    if (player->getArmies() > 0) {
        // Deploy most armies to weakest country;
        int armies = player->getArmies() * 0.5 + 5;
        if (armies > player->getArmies()) {
            armies = player->getArmies();
        }
        player->issueDeployOrder(defend[0], armies);
        getContinueInput();
        return false;
    }

    for (auto &dest : defend) {
        vector<Territory *> sortedByStrongest = sortTerritoriesByArmies(
                setToVector(player->getOwnedTerritories()), true);
        for (auto &origin : sortedByStrongest) {
            if (origin != dest && map->areAdjacent(origin, dest)
                && origin->getArmies() > dest->getArmies()
                && origin->getAvailableArmies() > 0) {
                // Send over most available armies to protect
                int armies = dest->getAvailableArmies() / 2 + 1;
                player->issueAdvanceOrder(origin, dest, armies);
                getContinueInput();
                return true;
            }
        }
    }
    getContinueInput();
    return true;
}

vector<Territory *> BenevolentPlayerStrategy::toAttack(Map *map) {
    // Never attacks
    return vector<Territory *>();
}

vector<Territory *> BenevolentPlayerStrategy::toDefend(Map *map) {
    return sortTerritoriesByArmies(setToVector(player->getOwnedTerritories()));
}

PlayerStrategy *BenevolentPlayerStrategy::clone() {
    return new BenevolentPlayerStrategy(*this);
}

ostream &BenevolentPlayerStrategy::print(ostream &out) const {
    out << "BenevolentPlayerStrategy";
    return out;
}

//=============================
// NeutralPlayerStrategy Implementation
//=============================

NeutralPlayerStrategy::NeutralPlayerStrategy(Player *player) : PlayerStrategy(player) {}

bool NeutralPlayerStrategy::issueOrder(Map *map, Deck *deck, vector<Player *> activePlayers) {
    // Never issues orders
    cout << "Employing [BenevolentPlayerStrategy] to issue orders" << endl;
    getContinueInput();
    return true;
}

vector<Territory *> NeutralPlayerStrategy::toAttack(Map *map) {
    // Never issues orders
    return vector<Territory *>();
}

vector<Territory *> NeutralPlayerStrategy::toDefend(Map *map) {
    // Never issues orders
    return vector<Territory *>();
}

PlayerStrategy *NeutralPlayerStrategy::clone() {
    return new NeutralPlayerStrategy(*this);
}

ostream &NeutralPlayerStrategy::print(ostream &out) const {
    out << "NeutralPlayerStrategy";
    return out;
}
