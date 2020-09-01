#include "check.h"
#include <algorithm>
using namespace std;

bool canBeNext(const Combo &a, const Combo &b)
{
    if (a.canBeCompared(b) && a.sameKind(b))
    {
        return a.greaterThan(b);
    }
    else
        return a.biggerSuit(b);
    return false;
}

const Combo& whatCombo(Poker* cards, int len){
    // 排序,是否有必要？
    sort(cards,cards+len);
    
    //统计同数字牌出现个数
    int rankBucket[15] = {0};
    Poker sings[20], pairs[20], trips[20], quarts[20];
    for(int i = 0; i<len; i++){
        rankBucket[cards[i].getRank()]++;
    }
    int sing_cnt = 0, pair_cnt = 0, trip_cnt = 0, quart_cnt = 0;
    for(int i = 0; i < 13; i++){
        switch (rankBucket[i])
        {
        case 0:break;
        case 1:sings[sing_cnt++]=Poker(i,spade);break;
        case 2:pairs[pair_cnt++]=Poker(i,spade);break;
        case 3:trips[trip_cnt++]=Poker(i,spade);break;
        case 4:quarts[quart_cnt++]=Poker(i,spade);break;
        default:break;
        }
    }   
    if(rankBucket[13]&&rankBucket[14]) pairs[pair_cnt++]=Poker(13,joker);

    // 单张、对子、三条 排序，为单顺、连对、飞机判断做准备
    sort(sings,sings+(sing_cnt));
    sort(pairs,pairs+(pair_cnt));
    sort(trips,trips+(trip_cnt));

    //牌型判断
    //单张
    if((sing_cnt==1) && (!pair_cnt) && (!trip_cnt) && (!quart_cnt))
    {
        return *(new Single(sings[0]));
    }
    //一对
    if ((!sing_cnt) && (pair_cnt==1) && (!trip_cnt) && (!quart_cnt))
    {
        if(pairs[0].getSuit()==joker) {
            return *(new Jokers());
        }
        return *(new Pair(pairs[0]));
    }
    //三不带
    if ((!sing_cnt) && (!pair_cnt) && (trip_cnt==1) && (!quart_cnt))
    {
        return *(new Triple(trips[0]));
    }
    //三带一
    if ((sing_cnt==1) && (!pair_cnt) && (trip_cnt == 1) && (!quart_cnt))
    {
        return *(new Triple(trips[0], 1, sings[0]));
    }
    //三带二
    if ((!sing_cnt) && (pair_cnt==1) && (trip_cnt==1) && (!quart_cnt))
    {
        return *(new Triple(trips[0], 2, pairs[0]));
    }
    //单顺
    if ((sing_cnt >= 5 && sing_cnt <=12) && (!pair_cnt) && (!trip_cnt) && (!quart_cnt))
    {
        if(isContinuous(sings,sing_cnt)) {
            return *(new Straight(sings,len));
        }
    }
    //连对
    if ((!sing_cnt) && (pair_cnt >=3 && pair_cnt <= 10) && (!trip_cnt) && (!quart_cnt))
    {
        if(isContinuous(pairs,pair_cnt)) {
            return *(new DStraight(pairs, pair_cnt));
        }
    }
    //四带二单张
    if ((sing_cnt == 2 && !pair_cnt) && (!trip_cnt) && (quart_cnt==1))
    {
        return *(new QuartWDoub(quarts[0],sings[0],sings[1],false));
    }
    //四带一对
    if ((!sing_cnt && pair_cnt==1) && (!trip_cnt) && (quart_cnt==1))
    {
        return *(new QuartWDoub(quarts[0], pairs[0], pairs[0], false));
    }
    //四带二对
    if ((!sing_cnt) && (pair_cnt==2) && (!trip_cnt) && (quart_cnt==1))
    {
        return *(new QuartWDoub(quarts[0], pairs[0], pairs[1], true));
    }
    //飞机不带翼
    if ((!sing_cnt) && (!pair_cnt) && (trip_cnt >= 2 && trip_cnt <=6) && (!quart_cnt))
    {
        if(isContinuous(trips,trip_cnt)) {
            return *(new Plane(trips,trip_cnt));
        }
    }
    // 飞机带单翼
    if ((sing_cnt == trip_cnt) && (!pair_cnt) && (trip_cnt >= 2 && trip_cnt <=6) && (!quart_cnt))
    {
        if(isContinuous(trips,trip_cnt)) return *(new Plane(trips,sings,trip_cnt,1));
    }
    // 飞机带双翼
    if ((!sing_cnt) && (pair_cnt==trip_cnt) && (trip_cnt >= 2 && trip_cnt <=6) && (!quart_cnt))
    {
        if(isContinuous(trips,trip_cnt)) return *(new Plane(trips,pairs,trip_cnt,2));
    }
    if((!sing_cnt) && (!pair_cnt) && (!trip_cnt) && (quart_cnt==1))
    {
        return *(new Bomb(quarts[0]));
    }
    return  *(new NotACombo());
}

bool isContinuous(Poker *cards, int len)
{
    for (int i = 1; i < len; i++)
    {
        //std::cout << cards[i].getRank() << " ";
        if (cards[i].getRank() != (cards[i - 1].getRank() + 1))
            return false;
        if (cards[i].getRank() >= TWO)
            return false;
    }
    //std::cout << std::endl;
    //std::cout << "con" << std::endl;
    return true;
}