#pragma once
#include <string>

class Card {
public:
    Card(const std::string& rank, const std::string& suit, const int& id);
    std::string toString() const;
    std::string getRank() const;
    std::string getSuit() const;
    int getID() const;
 
private:
    std::string rank;
    std::string suit;
    int ID;
};