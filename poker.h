#pragma once
enum suit{
    diamond,    // 0 
    club,
    heart,
    spade,
    joker,      // 4
};

#define THREE 0
#define FOUR 1
#define FIVE 2
#define SIX 3
#define SEVEN 4
#define EIGHT 5
#define NINE 6
#define TEN 7
#define JACK 8
#define QUEEN 9
#define KING 10
#define ACE 11
#define TWO 12
#define SMALL_JOKER 13
#define BIG_JOKER 14

typedef int card_rank;
// 0-12: 345678910JQK12  13: sJoker 14:bJoker

class Poker {
    card_rank _rank;
    suit _suit;
public:
    Poker();
    Poker(card_rank,suit);
    card_rank getRank() const {return _rank;}
    suit getSuit() const {return _suit;}
    bool operator<(const Poker&) const;    
    bool operator==(const Poker&) const;
    bool operator<=(const Poker&) const;
    bool operator>(const Poker&) const;
};