#include "GameObservers.h"

#include "../utils/Utils.h"
#include "../GameEngine.h"


using cris_utils::removeElement;
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
// Subject Implementation
//=============================


Subject::Subject() : observers{} {}

Subject::Subject(const Subject &other) : observers{} {
    for (auto &observer : other.observers) {
        observers.push_back(observer->clone());
    }
}

void swap(Subject &a, Subject &b) {
    using std::swap;
    swap(a.observers, b.observers);
}

ostream &operator<<(ostream &out, const Subject &obj) {
    obj.print(out);
    return out;
}

void Subject::attach(Observer *observer) {
    observers.push_back(observer);
}

void Subject::detach(Observer *observer) {
    removeElement(observers, observer);
}

void Subject::notify() {
    for (auto &observer : observers) {
        observer->update();
    }
}

Subject::~Subject() {
    // Subject is responsible for its observers
    for (auto &observer : observers) {
        delete observer;
    }
}


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
    // TODO implement
}

void PhaseObserver::print(ostream &out) const {
    out << "PhaseObserver{ game:" << game << " }";
}

Observer *PhaseObserver::clone() {
    return new PhaseObserver(*this);
}

// This observer is not responsible for managing the game's memory, so it won't delete it
PhaseObserver::~PhaseObserver() {}


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

void GameStatisticsObserver::update() {
    // TODO implement
}

void GameStatisticsObserver::print(ostream &out) const {
    out << "GameStatisticsObserver{ game:" << game << " }";
}

Observer *GameStatisticsObserver::clone() {
    return new GameStatisticsObserver(*this);
}

// This observer is not responsible for managing the game's memory, so it won't delete it
GameStatisticsObserver::~GameStatisticsObserver() {}
