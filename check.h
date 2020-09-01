#pragma once
#include "poker.h"
#include "combo.h"

// returns whether B can be followed by A
bool canBeNext(const Combo& a, const Combo&b);

// interpret the list of cards into a Combo
const Combo& whatCombo(Poker* cards, int len);

bool isContinuous(Poker*,int);