
#include "Player.h"
#include "../orders/Orders.h"
#include "../cards/Cards.h"
#include "../map/Map.h"

using std::cout;
using std::endl;

/**
 * Driver for Player
 */
int main() {
    cout << std::boolalpha;

    auto deck = new Deck{};
    Player *player1 = new Player{"Player 1"};
    Player *player2 = new Player{"Player 2"};
    Continent *c0 = new Continent{"c0", 1};
    Territory *t0 = new Territory{"t0", c0, 0};
    Territory *t1 = new Territory{"t1", c0, 0};

    player1->captureTerritory(t0);
    player1->captureTerritory(t1);


    auto *bombCard = new BombCard();
    deck->addCard(bombCard);
    deck->draw(player1->getHand());

    vector<Territory *> toDefend = player1->toDefend();
    vector<Territory *> toAttack = player1->toAttack(nullptr);

    player1->issueDeployOrder(10, t0);
    player1->issueAdvanceOrder(10, t0, t1);
    player1->issueCardOrder(deck, bombCard, 10, t0, t1, player2);

    cout << *player1;

    delete deck;
}