#include "poker.h"

/*
单张    single      1           /
一对    pair        2           /
三带    triple      3           带with  none/one/two 0-2
单顺    straight    5-12        /
连对    dstraight   2*3-2*10    /      
四带二  quartwdoub  4           带with  two/pair 2/4
飞机    plane       3*2-3*6     翼wing single/pair n/2*n
炸弹    bomb        4
王炸    jokers
*/

enum comboCat{
    single,pair,triple,straight,dstraight,quartwdoub,plane,bomb,jokers
};

class Combo{
    comboCat _catog;
    public:
        Combo(comboCat cat);
        bool operator==(const Combo&) const;
        bool operator<=(const Combo&) const;
};

class Single : public Combo{
    Poker card;
    public:
        Single(Poker);
        bool operator<=(const Single&) const;
};

class Pair : public Combo{
    Poker card;
    public:
        Pair(Poker);
        bool operator<=(const Pair&) const;    
};
