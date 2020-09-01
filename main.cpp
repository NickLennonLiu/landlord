#include "poker.h"
#include "combo.h"
#include "check.h"
#include <iostream>
using namespace std;

int main()
{
    Poker cards[15];
    for(int i = 0;i<13;i++){
        cards[i] = Poker(i,spade);
    }
    cards[13] = Poker(13,joker);
    cards[14] = Poker(14,joker);

    Poker hearts[15];
    for (int i = 0; i < 13; i++)
    {
        hearts[i] = Poker(i, heart);
    }
    hearts[13] = Poker(13, joker);
    hearts[14] = Poker(14, joker);

    Poker sin[1] = {cards[3]},
          pai[2] = {cards[3], hearts[3]},
          triplewith1[4] = {cards[3], cards[4], hearts[3], hearts[3]},
          straigh[5] = {cards[0], cards[1], cards[2], cards[3], cards[4]},
          dstraigh[6] = {cards[0], hearts[0], cards[1], hearts[1], cards[2], hearts[2]},
          qwd[8] = {cards[0], hearts[0], hearts[0], hearts[0], cards[1], cards[1], hearts[2], hearts[2]},
          plan[8] = {cards[0], hearts[0], hearts[0], hearts[1], cards[1], cards[1], hearts[2], hearts[3]},
          bom[4] = {cards[0], hearts[0], hearts[0], hearts[0]},
          jok[2] = {cards[13],cards[14]};
    const Combo& combo = whatCombo(sin,1); cout << combo << endl;
    const Combo &combo1 = whatCombo(pai, 2);
    cout << combo1 << endl;
    const Combo &combo2 = whatCombo(triplewith1, 4);
    cout << combo2 << endl;
    const Combo &combo3 = whatCombo(straigh, 5);
    cout << combo3 << endl;
    const Combo &combo4 = whatCombo(dstraigh, 6);
    cout << combo4 << endl;
    const Combo &combo5 = whatCombo(qwd, 8);
    cout << combo5 << endl;
    const Combo &combo6 = whatCombo(plan, 8);
    cout << combo6 << endl;
    const Combo &combo7 = whatCombo(bom, 4);
    cout << combo7 << endl;
    const Combo &combo8 = whatCombo(jok, 2);
    cout << combo8 << endl;

    /* Test Comparators
    Combo * combs[13] = {
        new Single(cards[4]),
        new Single(cards[3]),
        new Pair(cards[4]),
        new Pair(cards[3]),
        new Triple(cards[4]),
        new Triple(cards[3]),
        new Triple(cards[5], 1, cards[6]),
        new Triple(cards[6], 1, cards[7]),
        new Triple(cards[8], 2, cards[9]),
        new Plane(&cards[3], &cards[7], 3, false),
        new Plane(&cards[2], &cards[5], 3, false),
        new Bomb(cards[1]),
        new Jokers()};

    for(int i = 0; i < 13;i++){
        for(int j = i;j < 13; j ++){
            std::cout << canBeNext(*combs[i],*combs[j]) << " ";
        }
        std::cout << std::endl;
    }
    */
    /* Test virtual funcs
    Single sing(cards[4]);
    Combo* ptoSing = &sing;

    Poker si = ptoSing->getCards();
    std::cout << si.getRank();
    */
    return 0;
}
