#include "GameEngine.h"


#include <filesystem>
#include <algorithm>
#include <random>

#include "utils/Utils.h"
#include "map/Map.h"
#include "player/Player.h"
#include "map-loader/MapLoader.h"
#include "cards/Cards.h"
#include "orders/Orders.h"


using std::cout;
using std::cin;
using std::endl;
using std::filesystem::recursive_directory_iterator;
using std::filesystem::directory_entry;
using std::filesystem::current_path;
using std::filesystem::path;

using cris_utils::getBoolInput;
using cris_utils::getIntInput;
using cris_utils::contains;
using cris_utils::printTitle;
using cris_utils::printSubtitle;
using cris_utils::removeElement;
using cris_utils::compare;
using cris_utils::vectorToSet;
using cris_utils::pickFromList;
using cris_utils::printList;

namespace {
    auto rd = std::random_device{};
    auto rng = std::default_random_engine{rd()};
}

Game::Game() : map{nullptr},
               activePlayers{},
               allPlayers{},
               deck{nullptr},
               gameOver{false} {}

Game::Game(const Game &other) :
        map{new Map(*other.map)},
        activePlayers{vector<Player *>()},
        deck{nullptr} {
    for (auto &player : other.activePlayers) {
        activePlayers.push_back(new Player(*player));
    }
}

void swap(Game &a, Game &b) {
    using std::swap;

    swap(a.map, b.map);
    swap(a.activePlayers, b.activePlayers);
}

Game &Game::operator=(Game other) {
    swap(*this, other);
    return *this;
}

ostream &operator<<(ostream &out, const Game &obj) {
    out << obj.map << endl;

    return out;
}

void Game::gameStart() {
    cout << endl;
    printTitle("Welcome to the game Risky Warfare!");

    string searchPath = current_path();

    // Finding available maps
    vector<string> maps{};
    for (auto &item : recursive_directory_iterator(searchPath)) {
        if (!item.is_directory() && item.path().extension().string() == ".map") {
            maps.push_back(item.path().string());
        }
    }

    // User picks a map. Map must be valid
    bool mapValid = true;
    do {
        if (!mapValid) {
            cout << "Map is invalid! Please pick another." << endl;
        }
        string mapPath = pickFromList("Maps available under" + searchPath, "Which map file do you want to load?", maps);
        map = MapLoader::readMapFile(mapPath, path(mapPath).filename());
        mapValid = map->validate();

    } while (!mapValid);


    // Create players
    cout << endl;
    int numPlayers = getIntInput("How many players are there?", 2, 5);

    for (int i = 1; i < numPlayers + 1; ++i) {
        allPlayers.push_back(new Player("Player " + std::to_string(i)));
    }
    activePlayers.insert(activePlayers.begin(), allPlayers.begin(), allPlayers.end());

    // Create deck of cards
    deck = new Deck();
    for (int i = 0; i < 5; ++i) {
        deck->addCard(new BombCard());
        deck->addCard(new ReinforcementCard());
        deck->addCard(new BlockadeCard());
        deck->addCard(new AirliftCard());
        deck->addCard(new DiplomacyCard());
    }


    // TODO Set up observers
//    cout << endl;
//    bool phaseObserver = getBoolInput("Do you want to turn on the phase observer?");
//    cout << endl;
//    bool gameStatsObserver = getBoolInput("Do you want to turn on the game statistics observer?");
}

void Game::startupPhase() {
    cout << endl;
    printTitle("Entering the startup phase!");

    // Determine order of play for players
    std::shuffle(activePlayers.begin(), activePlayers.end(), rng);
    cout << "Here is the order of players:" << endl;
    printList(activePlayers);
    cout << endl;

    // Assign territories round robin style
    int currentPlayer = 0;
    vector<Territory *> shuffledTerritories = map->getTerritories();
    std::shuffle(shuffledTerritories.begin(), shuffledTerritories.end(), rng);
    for (auto &territory : shuffledTerritories) {
        activePlayers[currentPlayer]->captureTerritory(territory);
        currentPlayer++;
        if (currentPlayer >= activePlayers.size()) {
            currentPlayer = 0;
        }
    }
    // Give initial armies to players
    int initialArmies = 0;
    switch (activePlayers.size()) {
        case 2:
            initialArmies = INITIAL_ARMIES_2P;
            break;
        case 3:
            initialArmies = INITIAL_ARMIES_3P;
            break;
        case 4:
            initialArmies = INITIAL_ARMIES_4P;
            break;
        case 5:
            initialArmies = INITIAL_ARMIES_5P;
            break;
    }

    cout << "Giving each player " << initialArmies << " armies at the start of the game!" << endl;
    for (auto &player : activePlayers) {
        player->addArmies(initialArmies);
    }
}

void Game::mainGameLoop() {
    cout << endl;
    printTitle("Entering the main game loop!");
    int rounds = 0;

    while (!gameOver) {
        reinforcementPhase();
        issueOrderPhase();
        executeOrdersPhase();

        checkGameState();
        gameOver = true;

        for (auto &player : activePlayers) {
            // Reset allies
            player->resetAllies();

            // Give card if captured territory.
            if (player->isCardDue()) {
                cout << player->getName() << " captured a territory this round! They will get a card" << endl;
                deck->draw(player->getHand());
                // Reset flag
                player->setCardDue(false);
            }


        }

    }

}

void Game::reinforcementPhase() {
    printSubtitle("Reinforcement phase");

    for (auto &player : activePlayers) {
        int numArmies = 0;
        set<Territory *> ownedTerritories = player->getOwnedTerritories();
        numArmies += ownedTerritories.size() / 3;
        set<Continent *> playerConts = map->getContinentsControlledByPlayer(player);
        for (auto &continent : playerConts) {
            numArmies += continent->getArmies();
        }
        cout << player->getName() << " owns " << ownedTerritories.size()
             << " territories, ";
        if (playerConts.empty()) {
            cout << "and control no continents";
        } else {
            cout << "and controls the following continents: " << endl;
        }
        for (auto &continent : playerConts) {
            cout << "\t- " << continent->getName() << "" << endl;
        }
        cout << endl << "They will receive " << numArmies << " armies" << endl << endl;
        player->addArmies(numArmies);
    }
}

void Game::issueOrderPhase() {
    printSubtitle("Order issuing phase");
    vector<bool> ready(activePlayers.size());
    while (contains(ready, false)) {
        for (int i = 0; i < activePlayers.size(); ++i) {
            if (!ready[i]) {
                activePlayers[i]->issueOrder(map, nullptr, activePlayers);
                ready[i] = getBoolInput("Are you done issuing orders?");
            }
        }
    }

}

void Game::executeOrdersPhase() {
    // TODO
    printSubtitle("Order execution Phase");
    for (auto player : activePlayers) {
        Order *order = player->getOrdersList()->getHighestPriorityOrder();

        cout << "Executing " << *order << endl;
        order->execute(map, player);

    }
}

void Game::checkGameState() {
    printSubtitle("Round over! Checking the game state!");

    for (auto &player : allPlayers) {
        if (contains(activePlayers, player) && player->getOwnedTerritories().empty()) {
            cout << player->getName() << " owns no territories. They will be eliminated" << endl;
            removeElement(activePlayers, player);
        }
    }

    for (auto &player : activePlayers) {
        if (vectorToSet(map->getTerritories()) == player->getOwnedTerritories()) {
            cout << *player << " won the game!";
            gameOver = true;
        }
    }
}


Game::~Game() {
    delete map;
    for (auto &player : activePlayers) {
        delete player;
    }
}







