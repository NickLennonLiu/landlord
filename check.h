#pragma once
#include "poker.h"
#include "combo.h"

// returns whether B can be followed by A
bool canBeNext(const Combo& a, const Combo&b);

// interpret the list of cards into a Combo
Combo& whatCombo(Poker* cards, int len);

// check valid straight/pair straight/plane
bool isContinuous(Poker*,int);