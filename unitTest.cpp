#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN  // doctest provides main()
#include "doctest.h"
#include "Card.h"
#include "Deck.h"
#include "GameplayLoop.h"
#include "util.h"

// ----------------------------------------------------------------------------
// Helper function to make a card using Card.h
// ----------------------------------------------------------------------------
static Card makeCard(const std::string& rank,
                     const std::string& suit,
                     int id = 0) {
    return Card(rank, suit, id);
}

// ----------------------------------------------------------------------------
// Game State testing
// ----------------------------------------------------------------------------

// foldFn
TEST_CASE("foldFn sets GameState to 2") {
    int GameState = 0;
    foldFn(GameState);
    CHECK(GameState == 2);
}

// Player WIN
TEST_CASE("gameStateFn player wins") {
    int playerMoney = 100, dealerMoney = 200, pot = 50;
    bool potAwarded = false;

    gameStateFn(1, playerMoney, dealerMoney, potAwarded, pot);

    CHECK(playerMoney == 150);   // + pot
    CHECK(dealerMoney == 200);
    CHECK(pot == 0);
    CHECK(potAwarded == true);
}

// Player LOSE
TEST_CASE("gameStateFn dealer wins") {
    int playerMoney = 100, dealerMoney = 200, pot = 50;
    bool potAwarded = false;

    gameStateFn(2, playerMoney, dealerMoney, potAwarded, pot);

    CHECK(playerMoney == 100);
    CHECK(dealerMoney == 250);   // + pot
    CHECK(pot == 0);
    CHECK(potAwarded == true);
}

// Game TIED
TEST_CASE("gameStateFn tie") {
    int playerMoney = 100, dealerMoney = 200, pot = 50;
    bool potAwarded = false;

    gameStateFn(3, playerMoney, dealerMoney, potAwarded, pot);

    CHECK(playerMoney == 125);   // + pot/2
    CHECK(dealerMoney == 225);   // + pot/2
    CHECK(pot == 0);
    CHECK(potAwarded == true);
}

// ----------------------------------------------------------------------------
// Checking basic Card initializations
// ----------------------------------------------------------------------------

TEST_CASE("Card.h functions") {
    Card c("A", "Spades", 10);
    CHECK(c.getRank() == "A");
    CHECK(c.getSuit() == "Spades");
    CHECK(c.getID() == 10);
    CHECK(c.toString() == "A of Spades");
}

// -----------------------------------------------------------------------------
// Checking Rank value mapping function
// -----------------------------------------------------------------------------

TEST_CASE("rankValue function checking from GameplayLoop.h") {
    CHECK(rankValue("2") == 2);
    CHECK(rankValue("10") == 10);
    CHECK(rankValue("K") == 13);
    CHECK(rankValue("A") == 14);
}

// -----------------------------------------------------------------------------
// Checking evaluateHand function
// -----------------------------------------------------------------------------

TEST_CASE("High Card Hand evaluation") {
    // A really bad hand
    std::vector<Card> hand = {
        makeCard("2", "Clubs", 0),
        makeCard("5", "Diamonds", 1),
        makeCard("9", "Hearts", 2),
        makeCard("J", "Spades", 3),
        makeCard("K", "Clubs", 4)
    };

    int score = evaluateHand(hand);
    CHECK(score > 0);
}

TEST_CASE("Basic Named hand evaluaitons") {
    // One Pair
    std::vector<Card> pairHand = {
        makeCard("9", "Clubs", 0),
        makeCard("9", "Diamonds", 1),
        makeCard("3", "Hearts", 2),
        makeCard("5", "Spades", 3),
        makeCard("K", "Clubs", 4)
    };
    int pairScore = evaluateHand(pairHand);

    // Two Pair
    std::vector<Card> twoPairHand = {
        makeCard("9", "Clubs", 0),
        makeCard("9", "Diamonds", 1),
        makeCard("5", "Hearts", 2),
        makeCard("5", "Spades", 3),
        makeCard("2", "Clubs", 4)
    };
    int twoPairScore = evaluateHand(twoPairHand);

    // Three of a Kind
    std::vector<Card> threeKindHand = {
        makeCard("4", "Clubs", 0),
        makeCard("4", "Diamonds", 1),
        makeCard("4", "Hearts", 2),
        makeCard("J", "Spades", 3),
        makeCard("K", "Clubs", 4)
    };
    int threeKindScore = evaluateHand(threeKindHand);

    // Straight
    std::vector<Card> straightHand = {
        makeCard("5", "Clubs", 0),
        makeCard("6", "Diamonds", 1),
        makeCard("7", "Hearts", 2),
        makeCard("8", "Spades", 3),
        makeCard("9", "Clubs", 4)
    };
    int straightScore = evaluateHand(straightHand);

    // Flush
    std::vector<Card> flushHand = {
        makeCard("2", "Hearts", 0),
        makeCard("5", "Hearts", 1),
        makeCard("9", "Hearts", 2),
        makeCard("J", "Hearts", 3),
        makeCard("K", "Hearts", 4)
    };
    int flushScore = evaluateHand(flushHand);

    // Full House
    std::vector<Card> fullHouseHand = {
        makeCard("3", "Clubs", 0),
        makeCard("3", "Diamonds", 1),
        makeCard("3", "Hearts", 2),
        makeCard("5", "Spades", 3),
        makeCard("5", "Clubs", 4)
    };
    int fullHouseScore = evaluateHand(fullHouseHand);

    // Four of a Kind
    std::vector<Card> fourKindHand = {
        makeCard("7", "Clubs", 0),
        makeCard("7", "Diamonds", 1),
        makeCard("7", "Hearts", 2),
        makeCard("7", "Spades", 3),
        makeCard("K", "Clubs", 4)
    };
    int fourKindScore = evaluateHand(fourKindHand);

    // Straight Flush
    std::vector<Card> straightFlushHand = {
        makeCard("5", "Clubs", 0),
        makeCard("6", "Clubs", 1),
        makeCard("7", "Clubs", 2),
        makeCard("8", "Clubs", 3),
        makeCard("9", "Clubs", 4)
    };
    int straightFlushScore = evaluateHand(straightFlushHand);

    // Royal Flush
    std::vector<Card> royalFlushHand = {
        makeCard("10", "Hearts", 0),
        makeCard("J", "Hearts", 1),
        makeCard("Q", "Hearts", 2),
        makeCard("K", "Hearts", 3),
        makeCard("A", "Hearts", 4)
    };
    int royalFlushScore = evaluateHand(royalFlushHand);

    // Checking if all these hands are correctly ranked
    CHECK(pairScore > 0);
    CHECK(twoPairScore > pairScore);
    CHECK(threeKindScore > twoPairScore);
    CHECK(straightScore > threeKindScore);
    CHECK(flushScore > straightScore);
    CHECK(fullHouseScore > flushScore);
    CHECK(fourKindScore > fullHouseScore);
    CHECK(straightFlushScore > fourKindScore);
    CHECK(royalFlushScore > straightFlushScore);
}

// Straight Case with A and 2 cards
TEST_CASE("Named hand evaluation of Straight with A, 2, 3, 4, 5") {
    std::vector<Card> straight = {
        makeCard("A", "Clubs", 0),
        makeCard("5", "Diamonds", 1),
        makeCard("4", "Hearts", 2),
        makeCard("3", "Spades", 3),
        makeCard("2", "Clubs", 4)
    };
    int score = evaluateHand(straight);

    // To compare with Straight with A and 2
    std::vector<Card> threeKindHand = {
        makeCard("4", "Clubs", 0),
        makeCard("4", "Diamonds", 1),
        makeCard("4", "Hearts", 2),
        makeCard("J", "Spades", 3),
        makeCard("K", "Clubs", 4)
    };
    int threeKindScore = evaluateHand(threeKindHand);

    CHECK(score > threeKindScore);
}

// -----------------------------------------------------------------------------
// Checking Deck
// -----------------------------------------------------------------------------

TEST_CASE("Deck creates 52 unique cards and deals them all") {
    Deck d;
    d.shuffle();

    bool cardThere[52] = {false};

    for (int i = 0; i < 52; ++i) {
        Card c = d.dealCard();
        int id = c.getID();
        CHECK(id >= 0);
        CHECK(id < 52);
        CHECK(cardThere[id] == false);
        cardThere[id] = true;
    }

    // 53rd call must throw
    CHECK_THROWS(d.dealCard());
}