
#include "Player.h"
#include "../orders/Orders.h"
#include "../cards/Cards.h"

using std::cout;
using std::endl;

int main() {
    auto deck = new Deck{};
    Player player{0, "Player 1"};

    player.addTerritory(0);
    player.addTerritory(1);
    player.addTerritory(2);
    player.addTerritory(3);

    auto *bombCard = new BombCard();
    deck->addCard(bombCard);
    deck->draw(player.getHand());

    vector<int> toDefend = player.toDefend();
    vector<int> toAttack = player.toAttack();

    player.issueOrder(10, 0);
    player.issueOrder(10, 0, 1);
    player.issueOrder(deck, player.getHand(), bombCard, 10, 0 , 1, 1);

    cout << player;

    delete deck;
}