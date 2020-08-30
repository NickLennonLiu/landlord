#include "combo.h"

Combo::Combo(comboCat cat)
    : _catog(cat)
{}

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

Single::Single(Poker card)
    : Combo(single)
    , card(card)
{
}

bool Single::operator<=(const Single& b) const{
    return card <= b.card;
}

Pair::Pair(Poker card)
    : Combo(pair)
    , card(card)
{
}

bool Pair::operator<=(const Pair& b) const{
    return card <= b.card;
}