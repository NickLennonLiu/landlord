#include "poker.h"
#include "combo.h"
#include <iostream>
//using namespace std;
/* 
template<class comb1, class comb2> bool canBeNext(comb1& a,comb2& b){
    Combo a1 = a, b1 = b;
    if(a1.canBeCompared(b1) && a.sameKind(b)){
        return a.greaterThan(b);
    } else {
        return (a1.biggerSuit(b1));
    }
    return false;
}
*/

bool canBeNext(Combo* a, Combo*b){
    Single *singleA, *singleB;
    Pair *pairA, *pairB;
    Triple *tripleA, *tripleB;
    Straight *straightA, *straightB;
    DStraight *dstraightA, *dstraightB;
    QuartWDoub *quartwdoubA, *quartwdoubB;
    Plane *planeA, *planeB;
    Bomb *bombA, *bombB;
    if(a->canBeCompared(*b)){
        switch (a->getCat()){
        case single:
            singleA = static_cast<Single*>(a);
            singleB = static_cast<Single*>(b);
            return (singleA->sameKind(*singleB) && singleA->greaterThan(*singleB));
            break;
        case pair:
            pairA = static_cast<Pair*>(a);
            pairB = static_cast<Pair*>(b);
            return (pairA->sameKind(*pairB) && pairA->greaterThan(*pairB));
            break;
        case triple:
            tripleA = static_cast<Triple*>(a);
            tripleB = static_cast<Triple*>(b);
            return (tripleA->sameKind(*tripleB) && tripleA->greaterThan(*tripleB));
            break;
        case straight:
            straightA = static_cast<Straight*>(a);
            straightB = static_cast<Straight*>(b);
            return (straightA->sameKind(*straightB) && straightA->greaterThan(*straightB));
            break;
        case dstraight:
            dstraightA = static_cast<DStraight*>(a);
            dstraightB = static_cast<DStraight*>(b);
            return (dstraightA->sameKind(*dstraightB) && dstraightA->greaterThan(*dstraightB));
            break;
        case quartwdoub:
            quartwdoubA = static_cast<QuartWDoub*>(a);
            quartwdoubB = static_cast<QuartWDoub*>(b);
            return (quartwdoubA->sameKind(*quartwdoubB) && quartwdoubA->greaterThan(*quartwdoubB));
            break;
        case plane:
            planeA = static_cast<Plane*>(a);
            planeB = static_cast<Plane*>(b);
            return (planeA->sameKind(*planeB) && planeA->greaterThan(*planeB));
            break;
        case bomb:
            bombA = static_cast<Bomb*>(a);
            bombB = static_cast<Bomb*>(b);
            return (bombA->sameKind(*bombB) && bombA->greaterThan(*bombB));
            break;
        
        default:
            break;
        }
    } else {
        return a->biggerSuit(*b);
    }
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

    /*
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
            std::cout << canBeNext(combs[i],combs[j]) << " ";
        }
        std::cout << std::endl;
    }*/

    Single sing(cards[4]);
    Combo* ptoSing = &sing;

    Poker si = ptoSing->getCards();
    std::cout << si.getRank();
    return 0;
}