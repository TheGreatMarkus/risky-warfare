#include "GameEngine.h"
#include <iostream>

using std::cout;


int main() {
    cout << std::boolalpha;
    Game game{};

    game.gameStart();
    game.startupPhase();
    game.mainGameLoop();
}