#include "poker.h"

Poker::Poker(card_rank rank,suit suit)
    : _rank(rank), _suit(suit)
    {
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