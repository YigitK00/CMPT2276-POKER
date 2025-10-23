#include "Card.h"

Card::Card(const std::string& rank, const std::string& suit, const int& id) : rank(rank), suit(suit), ID(id) {}

std::string Card::toString() const {
    return rank + " of " + suit;
}

std::string Card::getRank() const {
    return rank;
}

std::string Card::getSuit() const {
    return suit;
}

int Card::getID() const {
    return ID;
}
