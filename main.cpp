#include <SFML/Graphics.hpp>
#include <iostream>
#include <algorithm>
#include "Deck.h"
#include "GameplayLoop.h"

std::string handName(int score);
int rankValue(const std::string& rank);
int evaluateHand(const std::vector<Card>& cards);
int main() {
    bool shouldDraw = false;  //THIS IS FOR THE HAND EVALUATION  

    bool isSelected[5] = { false, false, false, false, false };
    bool drawOutline[5] = { false, false , false, false, false };

    unsigned int cardsSelected = 0;


    sf::RenderWindow window(sf::VideoMode({ 1280u, 720u }), "Poker");
    //Raise, fold buttons

    sf::RectangleShape raise(sf::Vector2f(100.f, 100.f));
    raise.setPosition(sf::Vector2f(50.f, 600.f));
    raise.setFillColor(sf::Color::Green);
    raise.setOutlineColor(sf::Color::Black);
    raise.setOutlineThickness(4);

    sf::RectangleShape fold(sf::Vector2f(100.f, 100.f));
    fold.setPosition(sf::Vector2f(50.f, 400.f));
    fold.setFillColor(sf::Color::Red);
    fold.setOutlineColor(sf::Color::Black);
    fold.setOutlineThickness(4);

    // Check/Call button
    sf::RectangleShape check(sf::Vector2f(100.f, 100.f));
    check.setPosition(sf::Vector2f(50.f, 200.f));
    check.setFillColor(sf::Color::Yellow);
    check.setOutlineColor(sf::Color::Black);
    check.setOutlineThickness(4);
    //New round button
    sf::RectangleShape reset(sf::Vector2f(100.f, 100.f));
    reset.setPosition(sf::Vector2f(640.f, 300.f));
    reset.setFillColor(sf::Color::Green);
    reset.setOutlineColor(sf::Color::Black);
    reset.setOutlineThickness(4);
    //Discard button
    sf::RectangleShape Discard(sf::Vector2f(100.f, 100.f));
    Discard.setPosition(sf::Vector2f(1100.f, 400.f));
    Discard.setFillColor(sf::Color::Yellow);
    Discard.setOutlineColor(sf::Color::Black);
    Discard.setOutlineThickness(4);
    // Hand_Rankings
    sf::RectangleShape Hand_Rankings(sf::Vector2f(100.f, 100.f));
    Hand_Rankings.setPosition(sf::Vector2f(1100.f, 600.f));
    Hand_Rankings.setFillColor(sf::Color::Yellow);
    Hand_Rankings.setOutlineColor(sf::Color::Black);
    Hand_Rankings.setOutlineThickness(4);

    sf::RectangleShape Help(sf::Vector2f(100.f, 100.f));
    Help.setPosition(sf::Vector2f(900.f, 600.f));
    Help.setFillColor(sf::Color::Green);
    Help.setOutlineColor(sf::Color::Black);
    Help.setOutlineThickness(4);

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
    foldT.setPosition(sf::Vector2f(50.f, 400.f));

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
    checkT.setPosition(sf::Vector2f(50.f, 200.f));
    //Text for Discard
    sf::Text Discard_Text(font);
    Discard_Text.setFont(font);
    Discard_Text.setString("Discard");
    Discard_Text.setCharacterSize(28);
    Discard_Text.setOutlineColor(sf::Color::Black);
    Discard_Text.setOutlineThickness(2);
    Discard_Text.setFillColor(sf::Color::White);
    Discard_Text.setPosition(sf::Vector2f(1100.f, 400.f));
    // Text for Hand Rankings
    sf::Text Hand_Rankings_Text(font);
    Hand_Rankings_Text.setFont(font);
    Hand_Rankings_Text.setString("HAND\n      \n Eval");
    Hand_Rankings_Text.setCharacterSize(28);
    Hand_Rankings_Text.setOutlineColor(sf::Color::Black);
    Hand_Rankings_Text.setOutlineThickness(2);
    Hand_Rankings_Text.setFillColor(sf::Color::White);
    Hand_Rankings_Text.setPosition(sf::Vector2f(1110.f, 600.f));

    sf::Text HelpText(font);
    HelpText.setFont(font);
    HelpText.setString("Suggest");
    HelpText.setCharacterSize(28);
    HelpText.setOutlineColor(sf::Color::Black);
    HelpText.setOutlineThickness(2);
    HelpText.setFillColor(sf::Color::White);
    HelpText.setPosition(sf::Vector2f(900.f, 600.f));


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
    playerMoneyText.setPosition(sf::Vector2(50.f, 50.f));


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
    int dealerMoney = 99999999;
    int pot = 0;
    bool discarded = 0;
    bool potAwarded = false;
    std::vector<bool> enabledCard(5, false);

    Deck deck;
    deck.shuffle();

    std::vector<Card> playerHand;
    std::vector<Card> dealerHand;

    for (int i = 0; i < 5; ++i) {
        playerHand.push_back(deck.dealCard());
    }
    for (int i = 0; i < 5; ++i) {
        dealerHand.push_back(deck.dealCard());
    }

    std::vector<sf::Texture> playerTextures;
    
    // Create Player Card 1
    sf::Texture card1;
    if (!card1.loadFromFile("./playing-cards-master/" + std::to_string(playerHand[0].getID() + 1) + ".png")) {
        std::cerr << "Failed to load font\n";
        return -1;
    }
    sf::Sprite card1S(card1);
    card1S.setPosition(sf::Vector2f(300, 600));
    card1S.setScale(sf::Vector2f(.5, .5));

    sf::RectangleShape c1_Outline(sf::Vector2f(242 / 2.f, 340 / 2.f));
    c1_Outline.setPosition(sf::Vector2f(300.f, 600.f));
    c1_Outline.setOutlineColor(sf::Color::Black);
    c1_Outline.setOutlineThickness(4);



    // Create Player Card 2
    sf::Texture card2;
    if (!card2.loadFromFile("./playing-cards-master/" + std::to_string(playerHand[1].getID() + 1) + ".png")) {
        std::cerr << "Failed to load font\n";
        return -1;
    }
    sf::Sprite card2S(card2);
    card2S.setPosition(sf::Vector2f(425, 600));
    card2S.setScale(sf::Vector2f(.5, .5));

    sf::RectangleShape c2_Outline(sf::Vector2f(242 / 2.f, 340 / 2.f));
    c2_Outline.setPosition(sf::Vector2f(425.f, 600.f));
    c2_Outline.setOutlineColor(sf::Color::Black);
    c2_Outline.setOutlineThickness(4);

    //Player Card 3
    sf::Texture card3;
    if (!card3.loadFromFile("./playing-cards-master/" + std::to_string(playerHand[2].getID() + 1) + ".png")) {
        std::cerr << "Failed to load font\n";
        return -1;
    }
    sf::Sprite card3S(card3);
    card3S.setPosition(sf::Vector2f(550, 600));
    card3S.setScale(sf::Vector2f(.5, .5));

    sf::RectangleShape c3_Outline(sf::Vector2f(242 / 2.f, 340 / 2.f));
    c3_Outline.setPosition(sf::Vector2f(550.f, 600.f));
    c3_Outline.setOutlineColor(sf::Color::Black);
    c3_Outline.setOutlineThickness(4);

    //Player Card 4
    sf::Texture card4;
    if (!card4.loadFromFile("./playing-cards-master/" + std::to_string(playerHand[3].getID() + 1) + ".png")) {
        std::cerr << "Failed to load font\n";
        return -1;
    }
    sf::Sprite card4S(card4);
    card4S.setPosition(sf::Vector2f(675, 600));
    card4S.setScale(sf::Vector2f(.5, .5));

    sf::RectangleShape c4_Outline(sf::Vector2f(242 / 2.f, 340 / 2.f));
    c4_Outline.setPosition(sf::Vector2f(675.f, 600.f));
    c4_Outline.setOutlineColor(sf::Color::Black);
    c4_Outline.setOutlineThickness(4);

    //Player Card 5
    sf::Texture card5;
    if (!card5.loadFromFile("./playing-cards-master/" + std::to_string(playerHand[4].getID() + 1) + ".png")) {
        std::cerr << "Failed to load font\n";
        return -1;
    }
    sf::Sprite card5S(card5);
    card5S.setPosition(sf::Vector2f(800, 600));
    card5S.setScale(sf::Vector2f(.5, .5));

    sf::RectangleShape c5_Outline(sf::Vector2f(242 / 2.f, 340 / 2.f));
    c5_Outline.setPosition(sf::Vector2f(800.f, 600.f));
    c5_Outline.setOutlineColor(sf::Color::Black);
    c5_Outline.setOutlineThickness(4);

    // Create Dealer Card 1
    sf::Texture Dcard1;
    if (!Dcard1.loadFromFile("./playing-cards-master/zLightBack.png")) {
        std::cerr << "Failed to load font\n";
        return -1;
    }
    sf::Sprite Dcard1S(Dcard1);
    Dcard1S.setPosition(sf::Vector2f(300, 0));
    Dcard1S.setScale(sf::Vector2f(.5, .5));


    // Create Dealer Card 2
    sf::Texture Dcard2;
    if (!Dcard2.loadFromFile("./playing-cards-master/zLightBack.png")) {
        std::cerr << "Failed to load font\n";
        return -1;
    }

    sf::Sprite Dcard2S(Dcard2);
    Dcard2S.setPosition(sf::Vector2f(425, 0));
    Dcard2S.setScale(sf::Vector2f(.5, .5));

    // Create Dealer Card 3
    sf::Texture Dcard3;
    if (!Dcard3.loadFromFile("./playing-cards-master/zLightBack.png")) {
        std::cerr << "Failed to load font\n";
        return -1;
    }

    sf::Sprite Dcard3S(Dcard3);
    Dcard3S.setPosition(sf::Vector2f(550, 0));
    Dcard3S.setScale(sf::Vector2f(.5, .5));


    // Create Dealer Card 4
    sf::Texture Dcard4;
    if (!Dcard4.loadFromFile("./playing-cards-master/zLightBack.png")) {
        std::cerr << "Failed to load font\n";
        return -1;
    }

    sf::Sprite Dcard4S(Dcard4);
    Dcard4S.setPosition(sf::Vector2f(675, 0));
    Dcard4S.setScale(sf::Vector2f(.5, .5));

    // Create Dealer Card 5
    sf::Texture Dcard5;
    if (!Dcard5.loadFromFile("./playing-cards-master/zLightBack.png")) {
        std::cerr << "Failed to load font\n";
        return -1;
    }

    sf::Sprite Dcard5S(Dcard5);
    Dcard5S.setPosition(sf::Vector2f(800, 0));
    Dcard5S.setScale(sf::Vector2f(.5, .5));

    sf::Texture handRankingImage;
    if (!handRankingImage.loadFromFile("i2.png")) {
        std::cerr << "Failed to load Hand Ranking Image\n";
        return -1;
    }

    sf::Sprite HandRanking(handRankingImage);
    HandRanking.setPosition(sf::Vector2f(100, 100));
    HandRanking.setScale(sf::Vector2f(2, 2));

    //Evaluate the player and the dealer hands


    int playerScore = evaluateHand(playerHand);
    int dealerScore = evaluateHand(dealerHand);

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


                    if (c1_Outline.getGlobalBounds().contains(
                        sf::Vector2f(static_cast<float>(mousePressed->position.x),
                            static_cast<float>(mousePressed->position.y))))

                    {
                        if (drawOutline[0] == true) {
                            drawOutline[0] = false;
                            cardsSelected--;
                            enabledCard[0] = false;
                        }
                        else if (cardsSelected < 3) {
                            drawOutline[0] = true;
                            cardsSelected++;
                            enabledCard[0] = true;
                        }
                    }

                    if (c2_Outline.getGlobalBounds().contains(
                        sf::Vector2f(static_cast<float>(mousePressed->position.x),
                            static_cast<float>(mousePressed->position.y))))

                    {
                        if (drawOutline[1] == true) {
                            drawOutline[1] = false;
                            cardsSelected--;
                            enabledCard[1] = false;
                        }
                        else if (cardsSelected < 3) {
                            drawOutline[1] = true;
                            cardsSelected++;
                            enabledCard[1] = true;
                        }
                    }

                    if (c3_Outline.getGlobalBounds().contains(
                        sf::Vector2f(static_cast<float>(mousePressed->position.x),
                            static_cast<float>(mousePressed->position.y))))

                    {
                        if (drawOutline[2] == true) {
                            drawOutline[2] = false;
                            cardsSelected--;
                            enabledCard[2] = false;
                        }
                        else if (cardsSelected < 3) {
                            drawOutline[2] = true;
                            cardsSelected++;
                            enabledCard[2] = true;
                        }
                    }

                    if (c4_Outline.getGlobalBounds().contains(
                        sf::Vector2f(static_cast<float>(mousePressed->position.x),
                            static_cast<float>(mousePressed->position.y))))

                    {
                        if (drawOutline[3] == true) {
                            drawOutline[3] = false;
                            cardsSelected--;
                            enabledCard[3] = false;
                        }
                        else if (cardsSelected < 3) {
                            drawOutline[3] = true;
                            cardsSelected++;
                            enabledCard[3] = true;
                        }
                    }

                    if (c5_Outline.getGlobalBounds().contains(
                        sf::Vector2f(static_cast<float>(mousePressed->position.x),
                            static_cast<float>(mousePressed->position.y))))

                    {
                        if (drawOutline[4] == true) {
                            drawOutline[4] = false;
                            cardsSelected--;
                            enabledCard[4] = false;
                        }
                        else if (cardsSelected < 3) {
                            drawOutline[4] = true;
                            cardsSelected++;
                            enabledCard[4] = true;
                        }
                    }
                    //discard button
                    if (Discard.getGlobalBounds().contains(
                        sf::Vector2f(static_cast<float>(mousePressed->position.x),
                            static_cast<float>(mousePressed->position.y))))
                    {
                        if (!discarded) {
                            for (int i = 0; i < 5; i++) {
                                if (enabledCard[i]) {
                                    playerHand[i] = deck.dealCard();
                                    drawOutline[i] = 0;
                                    cardsSelected--;
                                    enabledCard[i] = 0;
                                }
                            }
                            discarded = 1;
                        }
                        playerScore = evaluateHand(playerHand);
                        playerhand.setString(handName(playerScore));
                        if (!card1.loadFromFile("./playing-cards-master/" + std::to_string(playerHand[0].getID() + 1) + ".png")) {
                            std::cerr << "Failed to load font\n";
                            return -1;
                        }
                        if (!card2.loadFromFile("./playing-cards-master/" + std::to_string(playerHand[1].getID() + 1) + ".png")) {
                            std::cerr << "Failed to load font\n";
                            return -1;
                        }
                        if (!card3.loadFromFile("./playing-cards-master/" + std::to_string(playerHand[2].getID() + 1) + ".png")) {
                            std::cerr << "Failed to load font\n";
                            return -1;
                        }
                        if (!card4.loadFromFile("./playing-cards-master/" + std::to_string(playerHand[3].getID() + 1) + ".png")) {
                            std::cerr << "Failed to load font\n";
                            return -1;
                        }
                        if (!card5.loadFromFile("./playing-cards-master/" + std::to_string(playerHand[4].getID() + 1) + ".png")) {
                            std::cerr << "Failed to load font\n";
                            return -1;
                        }
                    }
                    //new game button
                    if (reset.getGlobalBounds().contains(
                        sf::Vector2f(static_cast<float>(mousePressed->position.x),
                            static_cast<float>(mousePressed->position.y))))
                    {
                        GameState = 0;
                        discarded = 0;
                        deck.shuffle();
                        for (int i = 0; i < 5; i++) {
                            playerHand[i] = deck.dealCard();
                            dealerHand[i] = deck.dealCard();
                        }
                        
                        playerScore = evaluateHand(playerHand);
                        dealerScore = evaluateHand(dealerHand);
                        dealerhand.setString(handName(dealerScore));
                        playerhand.setString(handName(playerScore));
                        if (!card1.loadFromFile("./playing-cards-master/" + std::to_string(playerHand[0].getID() + 1) + ".png")) {
                            std::cerr << "Failed to load font\n";
                            return -1;
                        }
                        if (!card2.loadFromFile("./playing-cards-master/" + std::to_string(playerHand[1].getID() + 1) + ".png")) {
                            std::cerr << "Failed to load font\n";
                            return -1;
                        }
                        if (!card3.loadFromFile("./playing-cards-master/" + std::to_string(playerHand[2].getID() + 1) + ".png")) {
                            std::cerr << "Failed to load font\n";
                            return -1;
                        }
                        if (!card4.loadFromFile("./playing-cards-master/" + std::to_string(playerHand[3].getID() + 1) + ".png")) {
                            std::cerr << "Failed to load font\n";
                            return -1;
                        }
                        if (!card5.loadFromFile("./playing-cards-master/" + std::to_string(playerHand[4].getID() + 1) + ".png")) {
                            std::cerr << "Failed to load font\n";
                            return -1;
                        }
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


        //window.draw(c1_Outline);
        //window.draw(c2_Outline);
        //window.draw(c3_Outline);
        //window.draw(c4_Outline);
        //window.draw(c5_Outline);

        if (shouldDraw)
            window.draw(HandRanking);

        if (drawOutline[0])
            window.draw(c1_Outline);

        if (drawOutline[1])
            window.draw(c2_Outline);

        if (drawOutline[2])
            window.draw(c3_Outline);
        if (drawOutline[3])
            window.draw(c4_Outline);

        if (drawOutline[4])
            window.draw(c5_Outline);


        //Draw Cards
        window.draw(card1S);
        window.draw(card2S);
        window.draw(card3S);
        window.draw(card4S);
        window.draw(card5S);
       
        

        window.draw(Dcard1S);
        window.draw(Dcard2S);
        window.draw(Dcard3S);
        window.draw(Dcard4S);
        window.draw(Dcard5S);


        //Buttons and Text for these 
        window.draw(raise);
        window.draw(fold);

        window.draw(foldT);
        window.draw(raiseT);

        window.draw(check);
        window.draw(checkT);

        window.draw(Hand_Rankings);
        window.draw(Hand_Rankings_Text);

        window.draw(Help);
        window.draw(HelpText);

        window.draw(Discard);
        window.draw(Discard_Text);



      
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
        if (GameState != 0) {
            window.draw(reset);
        }

        window.display();
    }
}
