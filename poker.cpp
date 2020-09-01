#include "poker.h"

Poker::Poker(){}

Poker::Poker(card_rank rank,suit suit)
    : _rank(rank), _suit(suit)
    {
        if(rank>=SMALL_JOKER) _suit = joker;
    }

bool Poker::operator<(const Poker& b) const {
    return _rank < b.getRank();
}
bool Poker::operator==(const Poker& b) const {
    return (_rank == b.getRank());
}
bool Poker::operator<=(const Poker& b) const {
    return operator<(b) || operator==(b);
}
bool Poker::operator>(const Poker& b) const {
    return !operator<=(b);
}

QString Poker::string() const{
    QString info;
    switch(_suit){
    case club:info+="C";break;
    case diamond:info+="D";break;
    case heart:info+="H";break;
    case spade:info+="S";break;
    case joker:info+="J";break;
    }
    info += QString::number(_rank);
    return info;
}

Poker::Poker(QString card){
    if(card[0]=='C') _suit = club;
    if(card[0]=='D') _suit = diamond;
    if(card[0]=='H') _suit = heart;
    if(card[0]=='S') _suit = spade;
    if(card[0]=='J') _suit = joker;
    card.remove(0,1);
    _rank = card.toInt();
}
