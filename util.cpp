#include "util.h"

void gameStateFn(int GameState, int &playerMoney, int &dealerMoney, bool &potAwarded, int &pot) {
    if (GameState == 1) {
        playerMoney += pot;
        pot = 0;
        potAwarded = true;
    }
    else if (GameState == 2) {
        dealerMoney += pot;
        pot = 0;
        potAwarded = true;
    }
    else if (GameState == 3) {
        playerMoney += pot / 2;
        dealerMoney += pot / 2;
        pot = 0;
        potAwarded = true;
    }
}

void foldFn(int &GameState) {
    GameState = 2;
}
