#include "poker.h"
#include "combo.h"
#include <iostream>
using namespace std;

template<class psl> bool canBeNext(psl& a,psl& b){
    Combo a1 = a, b1 = b;
    if(a1 <= b1){
        cout << "same kind of combo"<<endl;
        return a<=b;
    }
    return false;
}

int main()
{
    Poker spadeACE(ACE,spade);
    Poker clubFIVE(FIVE,club);
    Single a(spadeACE),c(clubFIVE);
    Pair b(clubFIVE);
    //cout << (static_cast<Combo>(a)<=static_cast<Combo>(c));
    cout << canBeNext(a,c);
    return 0;
}