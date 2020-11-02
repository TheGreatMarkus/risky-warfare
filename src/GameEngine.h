#pragma once

#include <iostream>
#include <vector>

using std::ostream;
using std::vector;

class Map;
class Player;

class Game {
    Map* map;
    vector<Player *> players;
public:
    Game();
    Game(const Game &other);
    friend void swap(Game &a, Game &b);
    Game &operator=(Game other);
    friend ostream &operator<<(ostream &out, const Game &obj);

    // Your methods here
    void startGame();

    ~Game();
};