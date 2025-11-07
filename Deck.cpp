#include "Deck.h"
#include <algorithm>
#include <random>
#include <ctime>

Deck::Deck() : currentIndex(0) {
    std::vector<std::string> suits = { "Clubs", "Diamonds", "Hearts", "Spades" };
    std::vector<std::string> ranks = { "2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K", "A" };
    int id = 0;

    for (const auto& suit : suits) {
        for (const auto& rank : ranks) {
            cards.emplace_back(rank, suit, id);
            id++;
        }
    }
}

void Deck::shuffle() {
    std::mt19937 rng(static_cast<unsigned>(std::time(nullptr))); // seed with current time
    std::shuffle(cards.begin(), cards.end(), rng);
    currentIndex = 0;
}

Card Deck::dealCard() {
    if (currentIndex < cards.size()) {
        return cards[currentIndex++];
    }
    else {
        throw std::out_of_range("No more cards in the deck");
    }
}