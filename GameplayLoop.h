#pragma once
#include <iostream>
#include <vector>
#include "Deck.h"
#include <random>
#include <string>
#include <algorithm>
#include <map>
#include <SFML/Graphics.hpp>

int evaluateHand(const std::vector<Card>& cards);
int rankValue(const std::string& rank);
void resetRound(int gameState, Deck& newDeck, std::vector<Card>& newPhand, std::vector<Card>& newDhand);
int dealerLogic(int dealerScore, sf::Text& decision);

//Game Logic Functions
std::string handName(int score) {
    if (score >= 900) return "Royal Flush";
    if (score >= 800) return "Straight Flush";
    if (score >= 700) return "Four of a Kind";
    if (score >= 600) return "Full House";
    if (score >= 500) return "Flush";
    if (score >= 400) return "Straight";
    if (score >= 300) return "Three of a Kind";
    if (score >= 200) return "Two Pair";
    if (score >= 100) return "One Pair";
    return "High Card";
}


int rankValue(const std::string& rank) {
    std::map<std::string, int> values = {
        {"2", 2}, {"3", 3}, {"4", 4}, {"5", 5}, {"6", 6}, {"7", 7},
        {"8", 8}, {"9", 9}, {"10", 10}, {"J", 11}, {"Q", 12}, {"K", 13}, {"A", 14}
    };
    return values[rank];
}

int evaluateHand(const std::vector<Card>& cards) {
    std::map<int, int> valueCount;
    std::map<std::string, std::vector<int>> suitToValues;
    std::vector<int> values;

    for (const auto& card : cards) {
        int val = rankValue(card.getRank());
        values.push_back(val);
        valueCount[val]++;
        suitToValues[card.getSuit()].push_back(val);
    }

    std::sort(values.begin(), values.end());
    values.erase(std::unique(values.begin(), values.end()), values.end());

    auto isStraight = [](const std::vector<int>& vals) {
        if (vals.size() < 5) return false;
        for (size_t i = 0; i <= vals.size() - 5; ++i) {
            bool straight = true;
            for (int j = 0; j < 4; ++j) {
                if (vals[i + j] + 1 != vals[i + j + 1]) {
                    straight = false;
                    break;
                }
            }
            if (straight) return true;
        }
        std::vector<int> wheel = { 2, 3, 4, 5, 14 };
        return std::includes(vals.begin(), vals.end(), wheel.begin(), wheel.end());
        };

    // Flush and Straight Flush
    for (const auto& pair : suitToValues) {
        std::string suit = pair.first;
        std::vector<int> valList = pair.second;

        if (valList.size() >= 5) {
            std::sort(valList.begin(), valList.end());
            valList.erase(std::unique(valList.begin(), valList.end()), valList.end());

            if (isStraight(valList)) {
                if (std::find(valList.begin(), valList.end(), 14) != valList.end() &&
                    std::find(valList.begin(), valList.end(), 10) != valList.end()) {
                    return 900; // Royal Flush
                }
                return 800 + valList.back(); // Straight Flush
            }
            return 500 + valList.back(); // Flush
        }
    }

    // Four of a Kind
    for (const auto& pair : valueCount) {
        int val = pair.first;
        int count = pair.second;
        if (count == 4) return 700 + val;
    }

    // Full House
    int threeVal = 0, twoVal = 0;
    for (const auto& pair : valueCount) {
        int val = pair.first;
        int count = pair.second;
        if (count >= 3 && val > threeVal) threeVal = val;
    }
    for (const auto& pair : valueCount) {
        int val = pair.first;
        int count = pair.second;
        if (count >= 2 && val != threeVal && val > twoVal) twoVal = val;
    }
    if (threeVal && twoVal) return 600 + threeVal;

    // Straight
    if (isStraight(values)) return 400 + values.back();

    // Three of a Kind
    for (const auto& pair : valueCount) {
        int val = pair.first;
        int count = pair.second;
        if (count == 3) return 300 + val;
    }

    // Two Pair
    std::vector<int> pairs;
    for (const auto& pair : valueCount) {
        int val = pair.first;
        int count = pair.second;
        if (count == 2) pairs.push_back(val);
    }
    if (pairs.size() >= 2) {
        std::sort(pairs.begin(), pairs.end(), std::greater<int>());
        return 200 + pairs[0];
    }

    // One Pair
    if (!pairs.empty()) return 100 + pairs[0];

    // High Card
    return values.back();
}

void resetRound(int gameState, Deck& newDeck, std::vector<Card>& newPhand, std::vector<Card>& newDhand)
{
    gameState = 0;
    newDeck.shuffle();
    for (int i = 0; i < 5; i++) {
        newPhand[i] = newDeck.dealCard();
    }
    for (int i = 0; i < 5; i++) {
        newDhand[i] = newDeck.dealCard();
    }
}

int dealerLogic(int dealerScore, sf::Text& decision)
{
    std::random_device rd; // obtain a random number from hardware
    std::mt19937 gen(rd()); // seed the generator
    std::uniform_int_distribution<> distr(1, 100); // define the range
    int dice = distr(gen);
    if (dealerScore < 106) {
        if (dice <= 75) {
            decision.setString("dealer's folded");
            return 0;
        }
        else{
            decision.setString("dealer's checked, they look unconfident");
            return 1;
        }
    }
    else if (dealerScore < 300) {
        if (dice <= 75) {
            decision.setString("dealer's checked, they look normal");
            return 1;
        }
        else {
            decision.setString("dealer's raised, they seems normal");
            return 2;
        }
    }
    else if (dealerScore < 400) {
        if (dice <= 50) {
            decision.setString("dealer's checked, they seems normal");
            return 1;
        }
        else {
            decision.setString("dealer's raised, they seems confident");
            return 2;
        }
    }
    else if (dealerScore < 500) {
        if (dice <= 25) {
            decision.setString("dealer's checked, they seems confident");
            return 1;
        }
        else {
            decision.setString("dealer's raised, they seems confident");
            return 2;
        }
    }
    else if (dealerScore > 500) {
        decision.setString("dealer's raised, they seems very confident");
        return 2;
    }
    return 0;
}
