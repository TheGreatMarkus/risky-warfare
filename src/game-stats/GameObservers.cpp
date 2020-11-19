#include "GameObservers.h"

//=============================
// Observer Implementation
//=============================

Observer::Observer() {

}

Observer::Observer(const Observer &other) {

}

void swap(Observer &a, Observer &b) {

}

ostream &operator<<(ostream &out, const Observer &obj) {
    return out;
}

Observer::~Observer() {

}

//=============================
// Subject Implementation
//=============================

Subject::Subject() {

}

void swap(Subject &a, Subject &b) {

}

ostream &operator<<(ostream &out, const Subject &obj) {
    return out;
}

Subject::~Subject() {

}

//=============================
// PhaseObserver Implementation
//=============================

PhaseObserver::PhaseObserver() {

}

void swap(PhaseObserver &a, PhaseObserver &b) {

}

PhaseObserver &PhaseObserver::operator=(PhaseObserver other) {
    swap(*this, other);
    return *this;
}

ostream &operator<<(ostream &out, const PhaseObserver &obj) {
    return out;
}

void PhaseObserver::update() {

}

PhaseObserver::~PhaseObserver() {

}

//=============================
// PhaseObserver Implementation
//=============================

GameStatisticsObserver::GameStatisticsObserver() {

}

void swap(GameStatisticsObserver &a, GameStatisticsObserver &b) {

}

GameStatisticsObserver &GameStatisticsObserver::operator=(GameStatisticsObserver other) {
    swap(*this, other);
    return *this;
}

ostream &operator<<(ostream &out, const GameStatisticsObserver &obj) {
    return out;
}

void GameStatisticsObserver::update() {

}

GameStatisticsObserver::~GameStatisticsObserver() {

}
