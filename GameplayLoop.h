#pragma once
#include <iostream>
#include <vector>
#include "Deck.h"
#include <random>
#include <string>
#include <algorithm>
#include <map>
#include <SFML/Graphics.hpp>
#include <thread> // Required for std::this_thread::sleep_for
#include <chrono> // Required for std::chrono::seconds, milliseconds, etc.

int evaluateHand(const std::vector<Card>& cards);
int rankValue(const std::string& rank);
void resetRound(int gameState, Deck& newDeck, std::vector<Card>& newPhand, std::vector<Card>& newDhand);
int dealerLogic(int dealerScore, sf::Text& decision);
int hintLogic(const int playerScore, sf::Text& hint, const bool discarded, const std::vector<Card> playerhand);
void revealCards(sf::Texture &Dcard, const std::vector<Card> &Dhand,int i);
void hideCard(sf::Texture& card);
int convertRankToInt(std::string rank);

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



void revealCards(sf::Texture& card, const std::vector<Card>& hand, int i) {

      std::ignore = card.loadFromFile("./playing-cards-master/" + std::to_string(hand[i].getID() + 1) + ".png");
}

void hideCard(sf::Texture& card)
{
    std::ignore = card.loadFromFile("./playing-cards-master/zLightBack.png");
}

int convertRankToInt(std::string rank)
{
    if (rank == "A") {
        return 14;
    }
    else if (rank == "K") {
        return 13;
    }
    else if (rank == "Q") {
        return 12;
    }
    else if (rank == "J") {
        return 11;
    }
    else
        return std::stoi(rank);

    return 0;
}

int hintLogic(const int playerScore, sf::Text& hintText, const bool discarded, const std::vector<Card> playerhand)
{
    std::string hint;
    int sameSuit[4] = { 0, 0, 0, 0 };
    bool straightDraw = false;
    std::vector<int> ranks;
    if (playerScore < 200) {
        hint += "Your hand is very weak. Consider folding unless pot is small.";
    }
    else if (playerScore < 300) {
        hint += "You only have a marginal hand. Play cautiously or check.";
    }
    else if (playerScore < 400) {
        hint += "You have a moderate hand. Calling is reasonable, but avoid big raises.";
    }
    else if (playerScore < 500) {
        hint += "Your hand is decent. You can call or make small raises.";
    }
    else if (playerScore < 600) {
        hint += "You have a strong hand. Aggressive play is recommended.";
    }
    else if (playerScore < 700) {
        hint += "Very strong hand. Raising is a solid option.";
    }
    else if (playerScore < 800) {
        hint += "Excellent handfeel confident. Consider big raises.";
    }
    else if (playerScore < 900) {
        hint += "Near-perfect hand. You should play aggressively.";
    }
    else {
        hint += "Royal Flush  bet as much as possible!";
    }
    for (int i = 0; i < 5; i++) {
        std::string temp;
        temp = playerhand[i].getSuit();
        if (temp == "Spades") {
            sameSuit[0] += 1;
        }
        else if (temp == "Diamonds") {
            sameSuit[1] += 1;
        }
        else if (temp == "Hearts") {
            sameSuit[2] += 1;
        }
        else {
            sameSuit[3] += 1;
        }
    }
    for (int i = 0; i < 4; i++) {
        if (sameSuit[i] == 3) {
            hint += " \nYou can also discard 2 cards for a potential flush";
            break;
        }
        else if (sameSuit[i] == 4) {
            hint += " \nYou can discard 1 card for flush";
            break;
        }
    }
    for (int i = 0; i < 5; i++) {
        ranks.push_back(convertRankToInt(playerhand[i].getRank()));
    }
    std::sort(ranks.begin(), ranks.end());
    ranks.erase(std::unique(ranks.begin(), ranks.end()), ranks.end());
    if (ranks.size() < 3) {
        straightDraw = false;
    }
    for (int i = 0; i < (int)ranks.size() - 1; i++) {
        int count = 1;
        int last = ranks[i];

        for (int j = i + 1; j < (int)ranks.size(); j++) {
            if (ranks[j] == last + 1) {
                count++;
                last = ranks[j];
            }
        }

        if (count >= 3) {
            straightDraw = true;
        }
    }
    if (straightDraw) {
        hint += " \nAnother option is to discard for straight";
    }
    
    hintText.setString(hint);

    return 0;
}