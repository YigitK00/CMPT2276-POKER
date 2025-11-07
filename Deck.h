#pragma once
#include "Card.h"
#include <vector>

class Deck {
public:
    Deck();
    void shuffle();
    Card dealCard();

private:
    std::vector<Card> cards;
    int currentIndex;
};