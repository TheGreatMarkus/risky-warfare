#include "GameObservers.h"

#include <iomanip>

#include "../utils/Utils.h"
#include "../GameEngine.h"
#include "../player/Player.h"
#include "../orders/Orders.h"

using cris_utils::removeElement;
using cris_utils::printSubtitle;
using cris_utils::contains;

using std::fixed;
using std::setprecision;



//=============================
// Observer Implementation
//=============================

Observer::Observer() {}

Observer::Observer(const Observer &other) {}

void swap(Observer &a, Observer &b) {}

ostream &operator<<(ostream &out, const Observer &obj) {
    obj.print(out);
    return out;
}

Observer::~Observer() {}

//=============================
// Observable Implementation
//=============================

Observable::Observable() : observers{} {}

Observable::Observable(const Observable &other) : observers{} {
    for (auto &observer : other.observers) {
        observers.push_back(observer->clone());
    }
}

void swap(Observable &a, Observable &b) {
    using std::swap;
    swap(a.observers, b.observers);
}

ostream &operator<<(ostream &out, const Observable &obj) {
    obj.print(out);
    return out;
}

void Observable::attach(Observer *observer) {
    observers.push_back(observer);
}

void Observable::detach(Observer *observer) {
    removeElement(observers, observer);
}

void Observable::notify() {
    for (auto &observer : observers) {
        observer->update();
    }
}

const vector<Observer *> &Observable::getObservers() const {
    return observers;
}

Observable::~Observable() {
    // Observable is responsible for its observers
    for (auto &observer : observers) {
        delete observer;
    }
}

//=============================
// PhaseObserver Implementation
//=============================


PhaseObserver::PhaseObserver(Game *game) : game{game} {}

// Since observer isn't responsible for managing the memory of the game, it won't clone it
PhaseObserver::PhaseObserver(const PhaseObserver &other) : game{other.game} {}

void swap(PhaseObserver &a, PhaseObserver &b) {
    using std::swap;
    swap(a.game, b.game);
}

PhaseObserver &PhaseObserver::operator=(PhaseObserver other) {
    swap(*this, other);
    return *this;
}

void PhaseObserver::update() {
    string title;
    title = "[PhaseObserver]: " + GamePhaseString[game->getPhase()];
    if (game->getCurrentPlayer() != nullptr) {
        title = title + ", " + game->getCurrentPlayer()->getName();
    }
    printSubtitle(title);
    cout << "Game phase overview:" << endl;

    if (game->getPhase() == ReinforcementPhase || game->getPhase() == IssuingPhase) {
        for (auto &player : game->getActivePlayers()) {
            cout << "\t" << player->getName() << ": " << player->getArmies() << " armies" << endl;
        }
    }
    if (game->getPhase() == IssuingPhase || game->getPhase() == ExecutingPhase) {
        for (auto &player : game->getActivePlayers()) {
            cout << "\tCurrent Orders for " << player->getName() << ":" << endl;
            for (int i = 0; i < player->getOrders()->size(); ++i) {
                cout << "\t\t- " << *(*player->getOrders())[i] << endl;
            }
        }
    }
}

void PhaseObserver::print(ostream &out) const {
    out << "PhaseObserver{ game:" << game << " }";
}

Observer *PhaseObserver::clone() {
    return new PhaseObserver(*this);
}

// This observer is not responsible for managing the game's memory, so it won't delete it
PhaseObserver::~PhaseObserver() {}

//=============================
// GameStatisticsObserver Implementation
//=============================

GameStatisticsObserver::GameStatisticsObserver(Game *game) : game{game} {}

// Since observer isn't responsible for managing the memory of the game, it won't clone it
GameStatisticsObserver::GameStatisticsObserver(const GameStatisticsObserver &other)
        : game{other.game} {}

void swap(GameStatisticsObserver &a, GameStatisticsObserver &b) {
    using std::swap;
    swap(a.game, b.game);
}

GameStatisticsObserver &GameStatisticsObserver::operator=(GameStatisticsObserver other) {
    swap(*this, other);
    return *this;
}

const int BAR_WIDTH = 40;

void GameStatisticsObserver::update() {
    cout << fixed << setprecision(0);
    printSubtitle("[GameStatisticsObserver]: World Domination Overview");
    Player *victor = nullptr;
    for (auto &player : game->getActivePlayers()) {
        double playerTerritories = player->getOwnedTerritories().size();
        double allTerritories = game->getMap()->getTerritories().size();
        double percentage = playerTerritories / allTerritories;
        int fill = BAR_WIDTH * percentage;
        int empty = BAR_WIDTH - fill;

        cout << "\t- " << player->getName() << ": "
             << string(fill, '#') << string(empty, '-') << " " << percentage * 100.0 << "%" << endl;
        if (percentage == 1) {
            victor = player;
        }
    }
    if (victor != nullptr) {
        cout << victor->getName() << " has won the game!" << endl;
    }
    cout << endl;

}

void GameStatisticsObserver::print(ostream &out) const {
    out << "GameStatisticsObserver{ game:" << game << " }";
}

Observer *GameStatisticsObserver::clone() {
    return new GameStatisticsObserver(*this);
}

// This observer is not responsible for managing the game's memory, so it won't delete it
GameStatisticsObserver::~GameStatisticsObserver() {}
