#pragma once

#include <iostream>
#include <string>

using std::ostream;
using std::string;

class Observer {
public:
    Observer();
    Observer(const Observer &other);
    friend void swap(Observer &a, Observer &b);
//    Observer &operator=(Observer other); // abstract class, can't overload assignment operator
    friend ostream &operator<<(ostream &out, const Observer &obj);

    virtual void update() = 0;


    virtual ~Observer();

};

class Subject {
    Subject();
    Subject(const Subject &other);
    friend void swap(Subject &a, Subject &b);
    //Order &operator=(Order other); // abstract class, can't overload assignment operator
    friend ostream &operator<<(ostream &out, const Subject &obj);

    virtual void notify() = 0;
    virtual string getState() = 0;

    virtual ~Subject();
};

class PhaseObserver : public Observer {
    PhaseObserver();
    PhaseObserver(const PhaseObserver &other);
    friend void swap(PhaseObserver &a, PhaseObserver &b);
    PhaseObserver &operator=(PhaseObserver other);
    friend ostream &operator<<(ostream &out, const PhaseObserver &obj);

    void update();


    virtual ~PhaseObserver();
};

class GameStatisticsObserver : public Observer {
    GameStatisticsObserver();
    GameStatisticsObserver(const GameStatisticsObserver &other);
    friend void swap(GameStatisticsObserver &a, GameStatisticsObserver &b);
    GameStatisticsObserver &operator=(GameStatisticsObserver other);
    friend ostream &operator<<(ostream &out, const GameStatisticsObserver &obj);

    void update();


    virtual ~GameStatisticsObserver();
};