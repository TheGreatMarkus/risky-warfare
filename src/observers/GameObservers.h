#pragma once

#include <iostream>
#include <string>
#include <vector>

using std::ostream;
using std::string;
using std::vector;

class Game;

class Observer {
public:
    friend ostream &operator<<(ostream &out, const Observer &obj);

    virtual void update() = 0;
    virtual void print(ostream &out) const = 0;
    virtual Observer *clone() = 0;
};

class Observable {
private:
    vector<Observer *> observers;
public:
    Observable();
    Observable(const Observable &other);
    friend void swap(Observable &a, Observable &b);
    // abstract class, can't overload assignment operator
    // Observable &operator=(Observable other);
    friend ostream &operator<<(ostream &out, const Observable &obj);

    void attach(Observer *observer);
    void detach(Observer *observer);
    void notify();

    virtual void print(ostream &out) const = 0;
    virtual Observable *clone() = 0;

    const vector<Observer *> &getObservers() const;
    virtual ~Observable();
};

class PhaseObserver : public Observer {
private:
    Game *game;
public:
    PhaseObserver(Game *game);
    PhaseObserver(const PhaseObserver &other);
    friend void swap(PhaseObserver &a, PhaseObserver &b);
    PhaseObserver &operator=(PhaseObserver other);

    void update() override;
    void print(ostream &out) const override;
    Observer *clone() override;

    virtual ~PhaseObserver();
};

class GameStatisticsObserver : public Observer {
private:
    Game *game;
public:
    GameStatisticsObserver(Game *game);
    GameStatisticsObserver(const GameStatisticsObserver &other);
    friend void swap(GameStatisticsObserver &a, GameStatisticsObserver &b);
    GameStatisticsObserver &operator=(GameStatisticsObserver other);

    void update() override;
    void print(ostream &out) const override;
    Observer *clone() override;

    virtual ~GameStatisticsObserver();
};
