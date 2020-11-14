#include "GameEngine.h"

#include <filesystem>

#include "map/Map.h"
#include "player/Player.h"
#include "map-loader/MapLoader.h"

using std::cout;
using std::cin;
using std::endl;
using std::filesystem::recursive_directory_iterator;
using std::filesystem::directory_entry;
using std::filesystem::current_path;

Game::Game() : map{nullptr}, players{vector<Player *>()}, deck{nullptr} {}

Game::Game(const Game &other) : map{new Map(*other.map)}, players{vector<Player *>()}, deck{nullptr} {
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
    cout << "Welcome to the game!" << endl;
    cout << "Looking for map in: " << current_path() << endl;
    vector<string> maps{};
    for (auto &item : recursive_directory_iterator(current_path())) {
        if (!item.is_directory() && item.path().extension().string() == ".map") {
            maps.push_back(item.path().string());
        }
    }
    cout << "Available Maps:" << endl;
    for (int i = 0; i < maps.size(); ++i) {
        cout << "Map " << i << ": " << maps[i] << endl;
    }

    cout << "Please choose a map to load: ";
    int chosenMap;
    cin >> chosenMap;

    map = MapLoader::readMapFile(maps[chosenMap], maps[chosenMap]);

    cout << "How many Players?";
    // TODO players

    // TODO observers

}

Game::~Game() {
    delete map;
    for (auto &player : players) {
        delete player;
    }
}
