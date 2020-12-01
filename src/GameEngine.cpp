#include "GameEngine.h"

#include <filesystem>
#include <algorithm>
#include <random>
#include <iostream>

#include "utils/Utils.h"
#include "map/Map.h"
#include "player/Player.h"
#include "map-loader/MapLoader.h"
#include "cards/Cards.h"
#include "orders/Orders.h"
#include "observers/GameObservers.h"
#include "player/PlayerStrategies.h"

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
using cris_utils::getContinueInput;

namespace {
    auto rd = std::random_device{};
    auto rng = std::default_random_engine{rd()};
}

//=============================
// Game Implementation
//=============================

Game::Game() : map{nullptr},
               activePlayers{},
               allPlayers{},
               deck{nullptr},
               mapLoader{nullptr},
               gameOver{false},
               phase{NoPhase},
               currentPlayer{nullptr} {}

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

void Game::print(ostream &out) const {

}

Observable *Game::clone() {
    return nullptr;
}

void Game::gameStart() {
    updateGameState(nullptr, GameStartPhase);
    phase = GameStartPhase;

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
    string warzoneOption = "Warzone Loader";
    string conquestOption = "Conquest Loader";
    vector<string> availableLoaders = {warzoneOption, conquestOption};
    do {
        if (!mapValid) {
            cout << "Map is invalid! Please pick another." << endl;
            delete mapLoader;
            delete map;
            mapLoader = nullptr;
            map = nullptr;
        }
        string mapPath = pickFromList("Maps available under" + searchPath,
                                      "Which map file do you want to load?", maps);
        string choice = pickFromList("Among the following loaders:",
                                     "Which loader should be used for the map you chose?", availableLoaders);
        if (choice == warzoneOption) {
            mapLoader = new MapLoader();
        } else {
            mapLoader = new ConquestFileReaderAdapter();
        }

        map = mapLoader->readMapFile(mapPath, path(mapPath).filename());
        mapValid = map->validate();

    } while (!mapValid);


    // Create players
    cout << endl;
    int numPlayers = getIntInput("How many players are there?", 2, 5);

    for (int i = 1; i < numPlayers + 1; ++i) {
        allPlayers.push_back(new Player("Player " + std::to_string(i)));
    }
    activePlayers.insert(activePlayers.begin(), allPlayers.begin(), allPlayers.end());

    vector<string> possibleStrategies{"Human Strategy", "Aggressive Strategy", "Benevolent Strategy",
                                      "Neutral Strategy"};
    for (auto &player : allPlayers) {
        cout << endl;
        string chosenStrategy = pickFromList("Among these possible player strategies:",
                                             "which strategy should " + player->getName() + " use?",
                                             possibleStrategies);
        if (chosenStrategy == possibleStrategies[0]) {
            player->setStrategy(new HumanPlayerStrategy(player));
            player->getHand()->addCard(new BombCard());
        } else if (chosenStrategy == possibleStrategies[1]) {
            player->setStrategy(new AggressivePlayerStrategy(player));
        } else if (chosenStrategy == possibleStrategies[2]) {
            player->setStrategy(new BenevolentPlayerStrategy(player));
        } else if (chosenStrategy == possibleStrategies[3]) {
            player->setStrategy(new NeutralPlayerStrategy(player));
        }
    }

    // Create deck of cards
    deck = new Deck();
    for (int i = 0; i < 5; ++i) {
        deck->addCard(new BombCard());
        deck->addCard(new ReinforcementCard());
        deck->addCard(new BlockadeCard());
        deck->addCard(new AirliftCard());
        deck->addCard(new DiplomacyCard());
    }

    cout << endl;
    bool phaseObserver = getBoolInput("Do you want to turn on the phase observer?");
    if (phaseObserver) {
        attach(new PhaseObserver(this));
    }
    cout << endl;
    bool gameStatsObserver = getBoolInput("Do you want to turn on the game statistics observer?");
    if (gameStatsObserver) {
        attach(new GameStatisticsObserver(this));
    }
}

void Game::startupPhase() {
    updateGameState(nullptr, StartupPhase);

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
    int initialArmies = INITIAL_ARMIES[activePlayers.size() - 2];

    cout << "Giving each player " << initialArmies << " armies at the start of the game!" << endl;
    for (auto &player : activePlayers) {
        player->addArmies(initialArmies);
    }
}

void Game::mainGameLoop() {
    cout << endl;
    printTitle("Entering the main game loop!");
    int round = 0;

    checkGameState();

    while (!gameOver) {
        for (auto &player : activePlayers) {
            player->setCardDue(false);
        }

        reinforcementPhase();
        issueOrderPhase();
        executeOrdersPhase();

        checkGameState();

        prepareNextRound();
        round++;
    }
}

void Game::reinforcementPhase() {
    updateGameState(nullptr, ReinforcementPhase);
    for (auto &player : activePlayers) {
        int numArmies = 0;
        set<Territory *> ownedTerritories = player->getOwnedTerritories();
        numArmies += ownedTerritories.size() / 3;
        set<Continent *> playerConts = map->getContinentsControlledByPlayer(player);

        cout << endl << player->getName() << " owns " << ownedTerritories.size()
             << " territories (+" << numArmies << "), ";

        if (playerConts.empty()) {
            cout << "and controls no continents. " << endl;
        } else {
            cout << "and controls the following continents: " << endl;
        }
        for (auto &continent : playerConts) {
            cout << "\t- " << continent->getName() << ": +" << continent->getArmies() << " armies" << endl;
            numArmies += continent->getArmies();
        }
        cout << player->getName() << " armies: " << player->getArmies()
             << " -> " << player->getArmies() + numArmies << " (+" << numArmies << ")" << endl;
        player->addArmies(numArmies);
    }
    getContinueInput();
}

void Game::issueOrderPhase() {
    vector<bool> ready(activePlayers.size());
    while (contains(ready, false)) {
        for (int i = 0; i < activePlayers.size(); ++i) {
            if (!ready[i]) {
                updateGameState(activePlayers[i], IssuingPhase);
                ready[i] = activePlayers[i]->issueOrder(map, deck, activePlayers);
            }
        }
    }

}

bool deployOrdersRemain(vector<Player *> players) {
    for (auto &player : players) {
        Order *order = player->getOrders()->getHighestPriorityOrder();
        if (order != nullptr && order->getType() == OrderType::DEPLOY) {
            return true;
        }
    }
    return false;
}

bool ordersRemain(vector<Player *> players) {
    for (auto &player : players) {
        if (!player->getOrders()->empty()) {
            return true;
        }
    }
    return false;
}

void Game::executeOrdersPhase() {
    while (ordersRemain(activePlayers)) {
        for (int i = 0; i < activePlayers.size(); ++i) {
            OrdersList *ordersList = activePlayers[i]->getOrders();
            if (ordersList->empty()) {
                cout << activePlayers[i]->getName() << " has no more orders to execute." << endl;
                continue;
            }

            updateGameState(activePlayers[i], ExecutingPhase);
            Order *order = ordersList->getHighestPriorityOrder();

            // If the order returned isn't a deploy, then current player doesn't have any deploy orders left.
            if (order->getType() != OrderType::DEPLOY && deployOrdersRemain(activePlayers)) {
                cout << "Cannot execute " << *order << ". Some Deploy orders haven't yet been executed." << endl;
                getContinueInput();
                continue;
            }
            order->execute(map, activePlayers[i]);

            // Remove order after executing
            ordersList->remove(order);
            delete order;

            order = ordersList->getHighestPriorityOrder();
            if (order == nullptr) {

            }
            getContinueInput();
        }
    }

}

void Game::checkGameState() {
    for (auto &player : allPlayers) {
        if (contains(activePlayers, player) && player->getOwnedTerritories().empty()) {
            cout << player->getName() << " owns no territories. They will be eliminated" << endl;
            removeElement(activePlayers, player);
        }
    }

    for (auto &player : activePlayers) {
        if (vectorToSet(map->getTerritories()) == player->getOwnedTerritories()) {
            cout << player->getName() << " won the game!" << endl;
            gameOver = true;
        }
    }
}

void Game::prepareNextRound() {
    for (auto &territory : map->getTerritories()) {
        territory->freeArmies();
    }

    updateGameState(nullptr, PrepareNextRoundPhase);

    for (auto &player : activePlayers) {
        // Reset allies
        player->resetAllies();

        // Give card if captured territory.
        if (player->isCardDue()) {
            cout << player->getName() << " captured a territory this round! They will get a card" << endl;
            deck->draw(player->getHand());
        }
    }
    getContinueInput();
}

void Game::updateGameState(Player *currentPlayer, GamePhase phase) {
    this->currentPlayer = currentPlayer;
    this->phase = phase;
    notify();
}

Map *Game::getMap() const {
    return map;
}

const vector<Player *> &Game::getActivePlayers() const {
    return activePlayers;
}

const vector<Player *> &Game::getAllPlayers() const {
    return allPlayers;
}

Deck *Game::getDeck() const {
    return deck;
}

bool Game::isGameOver() const {
    return gameOver;
}

GamePhase Game::getPhase() const {
    return phase;
}

Player *Game::getCurrentPlayer() const {
    return currentPlayer;
}

Game::~Game() {
    delete map;
    for (auto &player : allPlayers) {
        delete player;
    }

    delete deck;
    delete mapLoader;
}

int main() {
    cout << std::boolalpha;

    Game *game = new Game();

    game->gameStart();
    game->startupPhase();
    game->mainGameLoop();
    delete game;
}





