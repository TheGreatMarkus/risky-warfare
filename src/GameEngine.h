#pragma once

#include <iostream>
#include <vector>

using std::ostream;
using std::vector;

class Map;
class Player;
class Deck;

class Game {
    Map* map;
    vector<Player *> players;
    Deck *deck;
public:
    Game();
    Game(const Game &other);
    friend void swap(Game &a, Game &b);
    Game &operator=(Game other);
    friend ostream &operator<<(ostream &out, const Game &obj);


    void initGame();
    void startupPhase();

    ~Game();
};