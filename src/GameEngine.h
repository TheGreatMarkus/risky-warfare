#pragma once

#include <iostream>
#include <vector>

#include "observers/GameObservers.h"

using std::ostream;
using std::vector;

class Map;
class Player;
class Deck;
class MapLoader;

/**
 * Describes the current phase of the game
 */
enum GamePhase {
    NoPhase,
    GameStartPhase,
    StartupPhase,
    ReinforcementPhase,
    IssuingPhase,
    ExecutingPhase,
    PrepareNextRoundPhase,
};

/**
 * God class containing the state to play a game
 */
class Game : public Observable {
    Map *map;
    vector<Player *> activePlayers;
    vector<Player *> allPlayers;
    Deck *deck;
    MapLoader *mapLoader;

    bool gameOver;
    GamePhase phase;
    Player *currentPlayer;
public:
    Game();
    Game(const Game &other);
    friend void swap(Game &a, Game &b);
    Game &operator=(Game other);

    void print(ostream &out) const override;
    Observable *clone() override;

    void gameStart();
    void startupPhase();
    void mainGameLoop();
    void reinforcementPhase();
    void issueOrderPhase();
    void executeOrdersPhase();
    void checkGameState();
    void prepareNextRound();
    void updateGameState(Player *currentPlayer, GamePhase phase);

    Map *getMap() const;
    const vector<Player *> &getActivePlayers() const;
    GamePhase getPhase() const;
    Player *getCurrentPlayer() const;

    ~Game();
};

/**
 * Describes the number of armies to give depending on the
 * number of players.
 *
 * Ex: INITIAL_ARMIES[2] = 40
 */
const int INITIAL_ARMIES[6] = {0, 0, 40, 35, 30, 25};

/**
 * String representation of each phase
 */
const vector<string> GamePhaseString = {
        "Not current in a Phase",
        "Game Start Phase",
        "Game Startup Phase",
        "Reinforcement Phase",
        "Issuing Orders Phase",
        "Order Execution Phase",
        "Preparing for next round",
};
