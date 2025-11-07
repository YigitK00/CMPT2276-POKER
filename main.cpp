#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include "Deck.h"
#include <string>

std::string handName(int score);
int rankValue(const std::string& rank);
int evaluateHand(const std::vector<Card>& cards);
int main() {
    bool shouldDraw = false;  //THIS IS FOR THE HAND EVALUATION   

    sf::RenderWindow window(sf::VideoMode({ 1280u, 720u }), "Poker");
    window.setFramerateLimit(60); //Performance reasons
    //Raise, fold buttons
    sf::RectangleShape raise(sf::Vector2f(100.f, 100.f));
    raise.setPosition(sf::Vector2f(50.f, 600.f));
    raise.setFillColor(sf::Color::Green);
    raise.setOutlineColor(sf::Color::Black);
    raise.setOutlineThickness(4);

    sf::RectangleShape fold(sf::Vector2f(100.f, 100.f));
    fold.setPosition(sf::Vector2f(200.f, 600.f));
    fold.setFillColor(sf::Color::Red);
    fold.setOutlineColor(sf::Color::Black);
    fold.setOutlineThickness(4);

    // Check/Call button
    sf::RectangleShape check(sf::Vector2f(100.f, 100.f));
    check.setPosition(sf::Vector2f(350.f, 600.f));
    check.setFillColor(sf::Color::Yellow);
    check.setOutlineColor(sf::Color::Black);
    check.setOutlineThickness(4);

    // Hand_Rankings
    sf::RectangleShape Hand_Rankings(sf::Vector2f(100.f, 100.f));
    Hand_Rankings.setPosition(sf::Vector2f(1100.f, 600.f));
    Hand_Rankings.setFillColor(sf::Color::Yellow);
    Hand_Rankings.setOutlineColor(sf::Color::Black);
    Hand_Rankings.setOutlineThickness(4);

    // load font
    sf::Font font;
    if (!font.openFromFile("ARIAL.TTF")) {
        return -1;
    }

    //Text for raise, fold buttons
    sf::Text foldT(font);
    foldT.setFont(font);
    foldT.setString("FOLD");
    foldT.setCharacterSize(28);
    foldT.setOutlineColor(sf::Color::Black);
    foldT.setOutlineThickness(2);
    foldT.setFillColor(sf::Color::White);
    foldT.setPosition(sf::Vector2f(210.f, 600.f));

    sf::Text raiseT(font);
    raiseT.setFont(font);
    raiseT.setString("RAISE");
    raiseT.setCharacterSize(28);
    raiseT.setOutlineColor(sf::Color::Black);
    raiseT.setOutlineThickness(2);
    raiseT.setFillColor(sf::Color::White);
    raiseT.setPosition(sf::Vector2f(60.f, 600.f));

    // Text for Check/Call button
    sf::Text checkT(font);
    checkT.setFont(font);
    checkT.setString("CHECK\n      /\n  CALL");
    checkT.setCharacterSize(28);
    checkT.setOutlineColor(sf::Color::Black);
    checkT.setOutlineThickness(2);
    checkT.setFillColor(sf::Color::White);
    checkT.setPosition(sf::Vector2f(350.f, 600.f));

    // Text for Hand Rankings
    sf::Text Hand_Rankings_Text(font);
    Hand_Rankings_Text.setFont(font);
    Hand_Rankings_Text.setString("HAND\n      \n Eval");
    Hand_Rankings_Text.setCharacterSize(28);
    Hand_Rankings_Text.setOutlineColor(sf::Color::Black);
    Hand_Rankings_Text.setOutlineThickness(2);
    Hand_Rankings_Text.setFillColor(sf::Color::White);
    Hand_Rankings_Text.setPosition(sf::Vector2f(1110.f, 600.f));


    // Text for pot
    sf::Text potText(font);
    potText.setFont(font);
    potText.setCharacterSize(32);
    potText.setFillColor(sf::Color::White);
    potText.setOutlineColor(sf::Color::Black);
    potText.setOutlineThickness(2);
    potText.setPosition(sf::Vector2(1050.f, 50.f));

    // Text for player money
    sf::Text playerMoneyText(font);
    playerMoneyText.setFont(font);
    playerMoneyText.setCharacterSize(28);
    playerMoneyText.setFillColor(sf::Color::White);
    playerMoneyText.setOutlineColor(sf::Color::Black);
    playerMoneyText.setOutlineThickness(2);
    playerMoneyText.setPosition(sf::Vector2(750.f, 600.f));


    //Set background table to table
    sf::Texture backgroundTexture;
    if (!backgroundTexture.loadFromFile("pokertable.jpg")) {
        std::cerr << "Failed to load font\n";
        return -1;
    }

    sf::Sprite background(backgroundTexture);

    // Scale Background Image to Screen.
    sf::Vector2u textureSize = backgroundTexture.getSize();
    sf::Vector2u windowSize = window.getSize();

    float xScale = float(windowSize.x) / textureSize.x;
    float yScale = float(windowSize.y) / textureSize.y;

    background.setScale(sf::Vector2f(xScale, yScale));

    // Game Start here
    int GameState = 0; // 0 means nothing, 1 is win, 2 is lose, 3 is tied
    int playerMoney = 100;
    int dealerMoney = 100;
    int pot = 0;
    bool potAwarded = false;

    Deck deck;
    deck.shuffle();

    std::vector<Card> playerHand = { deck.dealCard(), deck.dealCard() };
    std::vector<Card> dealerHand = { deck.dealCard(), deck.dealCard() };
    std::vector<Card> communityCards;

    for (int i = 0; i < 5; ++i) {
        communityCards.push_back(deck.dealCard());
    }


    // Create Player Card 1
    sf::Texture card1;
    if (!card1.loadFromFile("./playing-cards-master/" + std::to_string(playerHand[0].getID() + 1) + ".png")) {
        std::cerr << "Failed to load font\n";
        return -1;
    }
    sf::Sprite card1S(card1);
    card1S.setPosition(sf::Vector2f(500, 600));
    card1S.setScale(sf::Vector2f(.5, .5));


    // Create Player Card 2
    sf::Texture card2;
    if (!card2.loadFromFile("./playing-cards-master/" + std::to_string(playerHand[1].getID() + 1) + ".png")) {
        std::cerr << "Failed to load font\n";
        return -1;
    }
    sf::Sprite card2S(card2);
    card2S.setPosition(sf::Vector2f(600, 600));
    card2S.setScale(sf::Vector2f(.5, .5));

    // Create Dealer Card 1
    sf::Texture Dcard1;
    if (!Dcard1.loadFromFile("./playing-cards-master/" + std::to_string(dealerHand[0].getID() + 1) + ".png")) {
        std::cerr << "Failed to load font\n";
        return -1;
    }
    sf::Sprite Dcard1S(Dcard1);
    Dcard1S.setPosition(sf::Vector2f(500, 0));
    Dcard1S.setScale(sf::Vector2f(.5, .5));

    // Create Dealer Card 2
    sf::Texture Dcard2;
    if (!Dcard2.loadFromFile("./playing-cards-master/" + std::to_string(dealerHand[1].getID() + 1) + ".png")) {
        std::cerr << "Failed to load font\n";
        return -1;
    }

    sf::Sprite Dcard2S(Dcard2);
    Dcard2S.setPosition(sf::Vector2f(600, 0));
    Dcard2S.setScale(sf::Vector2f(.5, .5));


    // Community Card 1
    sf::Texture ccard1;
    if (!ccard1.loadFromFile("./playing-cards-master/" + std::to_string(communityCards[0].getID() + 1) + ".png")) {
        std::cerr << "Failed to load font\n";
        return -1;
    }

    sf::Sprite ccard1S(ccard1);
    ccard1S.setPosition(sf::Vector2f(250, 270));
    ccard1S.setScale(sf::Vector2f(.5, .5));

    // Community 2
    sf::Texture ccard2;
    if (!ccard2.loadFromFile("./playing-cards-master/" + std::to_string(communityCards[1].getID() + 1) + ".png")) {
        std::cerr << "Failed to load font\n";
        return -1;
    }
    sf::Sprite ccard2S(ccard2);
    ccard2S.setPosition(sf::Vector2f(400, 270));
    ccard2S.setScale(sf::Vector2f(.5, .5));

    // Community 3
    sf::Texture ccard3;
    if (!ccard3.loadFromFile("./playing-cards-master/" + std::to_string(communityCards[2].getID() + 1) + ".png")) {
        std::cerr << "Failed to load font\n";
        return -1;
    }

    sf::Sprite ccard3S(ccard3);
    ccard3S.setPosition(sf::Vector2f(550, 270));
    ccard3S.setScale(sf::Vector2f(.5, .5));

    sf::Texture ccard4;
    if (!ccard4.loadFromFile("./playing-cards-master/" + std::to_string(communityCards[3].getID() + 1) + ".png")) {
        std::cerr << "Failed to load font\n";
        return -1;
    }

    sf::Sprite ccard4S(ccard4);
    ccard4S.setPosition(sf::Vector2f(700, 270));
    ccard4S.setScale(sf::Vector2f(.5, .5));



    sf::Texture ccard5;
    if (!ccard5.loadFromFile("./playing-cards-master/" + std::to_string(communityCards[4].getID() + 1) + ".png")) {
        std::cerr << "Failed to load font\n";
        return -1;
    }

    sf::Sprite ccard5S(ccard5);
    ccard5S.setPosition(sf::Vector2f(850, 270));
    ccard5S.setScale(sf::Vector2f(.5, .5));

    sf::Texture handRankingImage;
    if (!handRankingImage.loadFromFile("i2.png")) {
        std::cerr << "Failed to load Hand Ranking Image\n";
        return -1;
    }

    sf::Sprite HandRanking(handRankingImage);
    HandRanking.setPosition(sf::Vector2f(100, 100));
    HandRanking.setScale(sf::Vector2f(2, 2));

    //Evaluate the player and the dealer hands

    std::vector<Card> playerTotal = playerHand;
    playerTotal.insert(playerTotal.end(), communityCards.begin(), communityCards.end());

    std::vector<Card> dealerTotal = dealerHand;
    dealerTotal.insert(dealerTotal.end(), communityCards.begin(), communityCards.end());
    
    int playerScore = evaluateHand(playerTotal);
    int dealerScore = evaluateHand(dealerTotal);

    //Setup an announce for win/lost/tie

    sf::Text result(font);
    result.setFont(font);
    result.setCharacterSize(40);
    result.setOutlineColor(sf::Color::Black);
    result.setOutlineThickness(5);
    result.setFillColor(sf::Color::White);
    result.setPosition(sf::Vector2f(320.f, 360.f));

    //Setup to display hands after playing
    sf::Text playerhand(font);
    playerhand.setFont(font);
    playerhand.setCharacterSize(40);
    playerhand.setString(handName(playerScore));
    playerhand.setOutlineColor(sf::Color::Black);
    playerhand.setOutlineThickness(5);
    playerhand.setFillColor(sf::Color::White);
    playerhand.setPosition(sf::Vector2f(560.f, 540.f));

    sf::Text dealerhand(font);
    dealerhand.setFont(font);
    dealerhand.setCharacterSize(40);
    dealerhand.setString(handName(dealerScore));
    dealerhand.setOutlineColor(sf::Color::Black);
    dealerhand.setOutlineThickness(5);
    dealerhand.setFillColor(sf::Color::White);
    dealerhand.setPosition(sf::Vector2f(560.f, 180.f));


    //Updating the game
    while (window.isOpen()) {

        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();

            if (const auto* mousePressed = event->getIf<sf::Event::MouseButtonPressed>()) {
                if (mousePressed->button == sf::Mouse::Button::Left) {

                    // Check if the click is inside the button
                    if (raise.getGlobalBounds().contains(
                        sf::Vector2f(static_cast<float>(mousePressed->position.x),
                            static_cast<float>(mousePressed->position.y))))
                    {
                        std::cout << "Raise clicked!\n";
                        raise.setFillColor(sf::Color::Blue);

                        pot += 100;
                        playerMoney -= 50;
                        dealerMoney -= 50;

                        if (playerScore > dealerScore) {
                            GameState = 1;
                        }
                        else if (playerScore < dealerScore) {
                            GameState = 2;
                        }
                        else
                            GameState = 3;
                    }

                    if (check.getGlobalBounds().contains(
                        sf::Vector2f(static_cast<float>(mousePressed->position.x),
                            static_cast<float>(mousePressed->position.y))))
                    {
                        std::cout << "Check/Call clicked!\n";
                        check.setFillColor(sf::Color::Blue);

                        pot += 50;
                        playerMoney -= 25;
                        dealerMoney -= 25;

                        // Example: update game state as a call
                        if (playerScore > dealerScore) {
                            GameState = 1; // player wins
                        }
                        else if (playerScore < dealerScore) {
                            GameState = 2; // dealer wins
                        }
                        else {
                            GameState = 3; // tie
                        }
                    }

                    if (Hand_Rankings.getGlobalBounds().contains(
                        sf::Vector2f(static_cast<float>(mousePressed->position.x),
                            static_cast<float>(mousePressed->position.y))))
                    {
                        std::cout << "Hand Eval clicked!\n";
                        check.setFillColor(sf::Color::Blue);

                        if (shouldDraw == false)
                            shouldDraw = true;
                        else
                            shouldDraw = false;
                    }

                    if (fold.getGlobalBounds().contains(
                        sf::Vector2f(static_cast<float>(mousePressed->position.x),
                            static_cast<float>(mousePressed->position.y))))

                    {
                        std::cout << "Fold clicked!\n";
                        fold.setFillColor(sf::Color::Blue);
                        GameState = 2;
                    }

                    if (Hand_Rankings.getGlobalBounds().contains(
                        sf::Vector2f(static_cast<float>(mousePressed->position.x),
                            static_cast<float>(mousePressed->position.y))))

                    {
                        std::cout << "Button clicked!\n";
                        Hand_Rankings.setFillColor(sf::Color::Blue);

                    }
                }
            }
        }
        //player wins
        if (GameState == 1) {
            playerMoney += pot;
            pot = 0;
            potAwarded = true;
        }
        //dealer wins
        else if (GameState == 2) {
            dealerMoney += pot;
            pot = 0;
            potAwarded = true;
        }
        //tie
        else if (GameState == 3) {
            playerMoney += pot / 2;
            dealerMoney += pot / 2;
            pot = 0;
            potAwarded = true;
        }

        //Update pot text
        potText.setString("Pot: $" + std::to_string(pot));

        //Update player money
        playerMoneyText.setString("Your Money: $" + std::to_string(playerMoney));

        window.clear();
        window.draw(background);

        //Draw Cards
        window.draw(card1S);
        window.draw(card2S);

        window.draw(Dcard1S);
        window.draw(Dcard2S);

        window.draw(ccard1S);
        window.draw(ccard2S);
        window.draw(ccard3S);
        window.draw(ccard4S);
        window.draw(ccard5S);

        //Buttons and Text for these 
        window.draw(raise);
        window.draw(fold);

        window.draw(foldT);
        window.draw(raiseT);

        window.draw(check);
        window.draw(checkT);

        window.draw(Hand_Rankings);
        window.draw(Hand_Rankings_Text);

        if (shouldDraw)
            window.draw(HandRanking);

        //Draw pot
        window.draw(potText);

        //Draw player money
        window.draw(playerMoneyText);

        //Draw Result
        if (GameState == 1) {
            result.setString("You Won!");
            result.setFillColor(sf::Color::Green);
            window.draw(result);
            window.draw(playerhand);
            window.draw(dealerhand);
            //Draw player money
            window.draw(playerMoneyText);
        }
        else if (GameState == 2) {
            result.setString("The Dealer Won");
            result.setFillColor(sf::Color::Red);
            window.draw(result);
            window.draw(playerhand);
            window.draw(dealerhand);
            //Draw player money
            window.draw(playerMoneyText);
        }
        else if (GameState == 3) {
            result.setString("It is a tied!");
            window.draw(result);
            window.draw(playerhand);
            window.draw(dealerhand);
            //Draw player money
            window.draw(playerMoneyText);
        }

        window.display();
    }
}

//Functions related to game logic
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