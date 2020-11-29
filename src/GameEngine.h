#pragma once

#include <iostream>
#include <vector>

#include "observers/GameObservers.h"

using std::ostream;
using std::vector;

class Map;
class Player;
class Deck;

enum GamePhase {
    NoPhase,
    GameStartPhase,
    StartupPhase,
    ReinforcementPhase,
    IssuingPhase,
    ExecutingPhase,
};

class Game : public Observable {
    Map *map;
    vector<Player *> activePlayers;
    vector<Player *> allPlayers;
    Deck *deck;

    bool gameOver;
    GamePhase phase;
    Player *currentPlayer;
public:
    Game();
    Game(const Game &other);
    friend void swap(Game &a, Game &b);
    Game &operator=(Game other);
    friend ostream &operator<<(ostream &out, const Game &obj);

    void print(ostream &out) const override;
    Observable *clone() override;

    void gameStart();
    void startupPhase();
    void mainGameLoop();
    void reinforcementPhase();
    void issueOrderPhase();
    void executeOrdersPhase();
    void checkGameState();
    void updateGameState(Player *currentPlayer, GamePhase phase);

    Map *getMap() const;
    const vector<Player *> &getActivePlayers() const;
    const vector<Player *> &getAllPlayers() const;
    Deck *getDeck() const;
    bool isGameOver() const;
    GamePhase getPhase() const;
    Player *getCurrentPlayer() const;

    ~Game();
};

const int INITIAL_ARMIES[4] = {40, 35, 30, 25};

const vector<string> GamePhaseString = {
        "Not current in a Phase",
        "Game Start Phase",
        "Game Startup Phase",
        "Reinforcement Phase",
        "Issuing Orders Phase",
        "Order Execution Phase"
};
