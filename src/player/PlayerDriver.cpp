
#include "Player.h"

using std::cout;
using std::endl;

int main() {
    Player player1{0, "Player 1"};

    player1.addTerritory(0);
    player1.addTerritory(1);
    player1.addTerritory(2);
    player1.addTerritory(3);
    cout << player1;

    vector<int> toDefend = player1.toDefend();
    vector<int> toAttack = player1.toAttack();

    player1.issueOrder(true, 10, 0, 1);
    player1.issueOrder(true, 10, 0, 1);

    return 0;
}