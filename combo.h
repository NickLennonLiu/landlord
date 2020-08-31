#pragma once
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
    Poker _cards[21];
    int _list_len;
    public:
        Combo(comboCat cat);
        Poker* cardList() const;
        bool canBeCompared(const Combo&) const;
        bool biggerSuit(const Combo&) const;
        comboCat getCat() const;
        virtual Poker getCards(){return Poker();}
        virtual bool sameKind(const Combo&) const = 0;
        virtual bool greaterThan(const Combo&) const = 0;
};

class Single : public Combo{
    Poker card;
    public:
        Single(Poker);
        bool sameKind(const Combo&) const override;
        bool greaterThan(const Combo& b) const override;
        Poker getCards() override;
};

class Pair : public Combo{
    Poker card;
    public:
        Pair(Poker);
        bool sameKind(const Combo &) const override;
        bool greaterThan(const Combo &b) const override;
};


class Triple : public Combo{
    Poker card;
    int with_num;
    Poker with;
    public:
        Triple(Poker tri);
        Triple(Poker tri, int with_num, Poker with);
        bool sameKind(const Combo &) const override;
        bool greaterThan(const Combo &b) const override;
};

class Straight : public Combo{
    Poker cards[13];
    int len;
    public:
        Straight(Poker[],int);
        bool sameKind(const Combo &) const override;
        bool greaterThan(const Combo &b) const override;
};

class DStraight : public Combo{
    Poker cards[10];
    int len;
    public:
        DStraight(Poker[],int);
        bool sameKind(const Combo &) const override;
        bool greaterThan(const Combo &b) const override;
};


class QuartWDoub : public Combo{
    Poker card;
    Poker doub;
    bool isPair;
    public:
        QuartWDoub(Poker,Poker,bool);
        bool sameKind(const Combo &) const override;
        bool greaterThan(const Combo &b) const override;
};

class Plane : public Combo{
    Poker cards[6];
    Poker wings[6];
    int len;
    bool isPair;
    public:
        Plane(Poker[],Poker[],int,bool);
        bool sameKind(const Combo &) const override;
        bool greaterThan(const Combo &b) const override;
};

class Bomb : public Combo{
    Poker card;
    public:
        Bomb(Poker);
        bool sameKind(const Combo &) const override;
        bool greaterThan(const Combo &b) const override;
};

class Jokers : public Combo{
    public:
        Jokers();
        bool sameKind(const Combo &) const override;
        bool greaterThan(const Combo &b) const override;
};