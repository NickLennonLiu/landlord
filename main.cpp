#include "poker.h"
#include "combo.h"
#include <iostream>

bool canBeNext(const Combo& a,const Combo& b)
{
    if(a.canBeCompared(b) && a.sameKind(b)){
        return a.greaterThan(b);
    } else return a.biggerSuit(b);
    return false;
}

int main()
{
    Poker spadeACE(ACE,spade);
    Poker clubFIVE(FIVE,club);

    Poker cards[15];
    for(int i = 0;i<13;i++){
        cards[i] = Poker(i,spade);
    }
    cards[13] = Poker(13,joker);
    cards[14] = Poker(14,joker);

    
    Combo* combs[13] = {
        new Single(cards[4]),
        new Single(cards[3]),
        new Pair(cards[4]),
        new Pair(cards[3]),
        new Triple(cards[4]),
        new Triple(cards[3]),
        new Triple(cards[5],1,cards[6]),
        new Triple(cards[6],1,cards[7]),
        new Triple(cards[8],2,cards[9]),
        new Plane(&cards[3],&cards[7],3,false),
        new Plane(&cards[2],&cards[5],3,false),
        new Bomb(cards[1]),
        new Jokers()
    };
    
    for(int i = 0; i < 13;i++){
        for(int j = i;j < 13; j ++){
            std::cout << canBeNext(*combs[i],*combs[j]) << " ";
        }
        std::cout << std::endl;
    }

    Single sing(cards[4]);
    Combo* ptoSing = &sing;

    Poker si = ptoSing->getCards();
    std::cout << si.getRank();
    return 0;
}