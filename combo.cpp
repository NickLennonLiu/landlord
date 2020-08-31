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
bool Single::sameKind(const Single& b) const {
    return true;
}

bool Single::greaterThan(const Single& b) const {
    return card > b.card;
}
//Pair
bool Pair::sameKind(const Pair& b) const {
    return true;
}

bool Pair::greaterThan(const Pair&b) const {
    return card > b.card;
}
//Triple
bool Triple::sameKind(const Triple& b) const {
    return with_num==b.with_num;
}

bool Triple::greaterThan(const Triple& b) const {
    return card > b.card;
}
//Straight
bool Straight::sameKind(const Straight& b) const {
    return len == b.len;
}

bool Straight::greaterThan(const Straight& b) const {
    return cards[0] > b.cards[0];
}
//DStraight
bool DStraight::sameKind(const DStraight &b) const
{
    return len == b.len;
}

bool DStraight::greaterThan(const DStraight &b) const
{
    return cards[0] > b.cards[0];
}
//QuartWDoub
bool QuartWDoub::sameKind(const QuartWDoub & b) const
{
    return isPair==b.isPair;
}

bool QuartWDoub::greaterThan(const QuartWDoub&b) const {
    return card>b.card;
}
//Plane
bool Plane::sameKind(const Plane& b) const {
    return (len==b.len && isPair==b.isPair);
}

bool Plane::greaterThan(const Plane& b) const {
    return cards[0] > b.cards[0];
}
//Bomb
bool Bomb::sameKind(const Bomb &b) const
{
    return true;
}

bool Bomb::greaterThan(const Bomb &b) const
{
    return card > b.card;
}
//Jokers
// no need to compare


Poker Single::getCards() {
    return card;
}