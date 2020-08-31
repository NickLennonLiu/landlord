#include "combo.h"


Combo::Combo(comboCat cat)
    : _catog(cat)
{}

/*
bool Combo::operator==(const Combo& b) const{
    return _catog == b._catog;
}

bool Combo::operator<=(const Combo& b) const{
    if(_catog < b._catog){
        if(b._catog >= bomb) return true;
        return false;
    }
    return _catog == b._catog;
}
*/

bool Combo::canBeCompared(const Combo& b) const{
    return _catog == b._catog;
}

bool Combo::biggerSuit(const Combo& b) const{
    return (_catog >= bomb && _catog > b._catog);
}


comboCat Combo::getCat() const {return _catog;}

// 派生类类构造函数

Single::Single(Poker card)
    : Combo(single)
    , card(card)
{
}

Pair::Pair(Poker card)
    : Combo(pair)
    , card(card)
{
}

Triple::Triple(Poker tri)
    : Combo(triple)
    , card(tri)
{
    with_num = 0;
}

Triple::Triple(Poker tri, int wi, Poker with)
    : Combo(triple)
    , card(tri)
    , with_num(wi)
    , with(with)
{

}

Straight::Straight(Poker* list,int len)
    : Combo(straight)
    , len(len)
{
    for(int i = 0;i<len;i++){
        cards[i] = list[i];
    }
}

DStraight::DStraight(Poker* list,int len)
    : Combo(straight)
    , len(len)
{
    for(int i = 0;i<len;i++){
        cards[i] = list[i];
    }
}

QuartWDoub::QuartWDoub(Poker card,Poker doub,bool is)
    : Combo(quartwdoub)
    , card(card)
    , doub(doub)
    , isPair(is)
{
}

Plane::Plane(Poker* p,Poker* w,int len,bool is)
    : Combo(plane)
    , len(len)
    , isPair(is)
{
    for(int i = 0;i<len;i++)
    {
        cards[i] = p[i];
        wings[i] = w[i];
    }
}

Bomb::Bomb(Poker b)
    : Combo(bomb)
    , card(b)
{
}

Jokers::Jokers()
    : Combo(jokers)
{}
// 派生类比较函数

//Single
bool Single::sameKind(const Combo& b) const {
    return true;
}

bool Single::greaterThan(const Combo &b) const
{
    const Single& b1 = dynamic_cast<const Single&>(b);
    return card > b1.card;
}
//Pair
bool Pair::sameKind(const Combo &b) const
{
    return true;
}

bool Pair::greaterThan(const Combo &b) const
{
    const Pair& b1 = dynamic_cast<const Pair&>(b);
    return card > b1.card;
}
//Triple
bool Triple::sameKind(const Combo &b) const
{
    const Triple &b1 = dynamic_cast<const Triple &>(b);
    return with_num==b1.with_num;
}

bool Triple::greaterThan(const Combo &b) const
{
    const Triple& b1 = dynamic_cast<const Triple&>(b);
    return card > b1.card;
}
//Straight
bool Straight::sameKind(const Combo &b) const
{
    const Straight &b1 = dynamic_cast<const Straight &>(b);
    return len == b1.len;
}

bool Straight::greaterThan(const Combo &b) const
{
    const Straight &b1 = dynamic_cast<const Straight &>(b);
    return cards[0] > b1.cards[0];
}
//DStraight
bool DStraight::sameKind(const Combo &b) const
{
    const DStraight &b1 = dynamic_cast<const DStraight &>(b);
    return len == b1.len;
}

bool DStraight::greaterThan(const Combo &b) const
{
    const DStraight& b1 = dynamic_cast<const DStraight&>(b);
    return cards[0] > b1.cards[0];
}
//QuartWDoub
bool QuartWDoub::sameKind(const Combo &b) const
{
    const QuartWDoub &b1 = dynamic_cast<const QuartWDoub &>(b);
    return isPair==b1.isPair;
}

bool QuartWDoub::greaterThan(const Combo &b) const
{
    const QuartWDoub& b1 = dynamic_cast<const QuartWDoub&>(b);
    return card>b1.card;
}
//Plane
bool Plane::sameKind(const Combo &b) const
{
    const Plane &b1 = dynamic_cast<const Plane &>(b);
    return (len==b1.len && isPair==b1.isPair);
}

bool Plane::greaterThan(const Combo &b) const
{
    const Plane& b1 = dynamic_cast<const Plane&>(b);
    return cards[0] > b1.cards[0];
}
//Bomb
bool Bomb::sameKind(const Combo &b) const
{
    return true;
}

bool Bomb::greaterThan(const Combo &b) const
{
    const Bomb& b1 = dynamic_cast<const Bomb&>(b);
    return card > b1.card;
}
//Jokers
bool Jokers::sameKind(const Combo &b) const{
    return true;
}

bool Jokers::greaterThan(const Combo &b) const {
    return false;
}


Poker Single::getCards() {
    return card;
}