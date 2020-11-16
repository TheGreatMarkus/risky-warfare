#include "GameEngine.h"

#include <filesystem>
#include <algorithm>

#include "map/Map.h"
#include "player/Player.h"
#include "map-loader/MapLoader.h"
#include "cards/Cards.h"

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

void Game::initGame() {
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

    cout << "Enter the number of players: ";

    int numPlayers;
    cin >> numPlayers;

    for (int i = 0; i < numPlayers; ++i) {
        players.push_back(new Player(i, "Player " + std::to_string(i + 1)));
    }
    // TODO observers


    deck = new Deck();

}

void Game::startupPhase() {
    // TODO Determine order of play for players
        std::random_shuffle(players.begin(), players.end());

    // TODO Assign territories round robin style
    for (auto &territory : map->getTerritories()) {
        cout << territory;
    }
    // TODO Give initial armies

}

Game::~Game() {
    delete map;
    for (auto &player : players) {
        delete player;
    }
}


