#pragma once

#include <iostream>
#include <vector>

using std::ostream;
using std::vector;

class Map;
class Player;
class Deck;

class Game {
    Map *map;
    vector<Player *> activePlayers;
    vector<Player *> allPlayers;
    Deck *deck;
    bool gameOver;
public:
    Game();
    Game(const Game &other);
    friend void swap(Game &a, Game &b);
    Game &operator=(Game other);
    friend ostream &operator<<(ostream &out, const Game &obj);


    void gameStart();
    void startupPhase();
    void mainGameLoop();
    void reinforcementPhase();
    void issueOrderPhase();
    void executeOrdersPhase();
    void checkGameState();

    ~Game();
};

const int INITIAL_ARMIES_2P = 40;
const int INITIAL_ARMIES_3P = 35;
const int INITIAL_ARMIES_4P = 30;
const int INITIAL_ARMIES_5P = 25;
