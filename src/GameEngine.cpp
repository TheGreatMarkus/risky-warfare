#include "GameEngine.h"

#include "map/Map.h"
#include "player/Player.h"

using std::endl;

Game::Game() : map{nullptr}, players{vector<Player *>()} {}

Game::Game(const Game &other) : map{new Map(*other.map)}, players{vector<Player *>()} {
    for (auto &player : other.players) {
        players.push_back(new Player(*player));
    }
}

void swap(Game &a, Game &b) {
    using std::swap;

    swap(a.map, b.map);
    swap(a.players, b.players);
}

Game &Game::operator=(Game other) {
    swap(*this, other);
    return *this;
}

ostream &operator<<(ostream &out, const Game &obj) {
    out << obj.map << endl;

    return out;
}

void Game::startGame() {

}

Game::~Game() {
    delete map;
    for (auto &player : players) {
        delete player;
    }
}
