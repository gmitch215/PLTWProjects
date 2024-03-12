/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       game.cpp                                                  */
/*    Author:       mitchellg2026                                             */
/*    Created:      Fri Mar 1 2024                                            */
/*    Description:  Implementation for Pop-A-Shot Game                        */
/*                                                                            */
/*----------------------------------------------------------------------------*/
#include "vex.h"
#include "game.h"
#include "scoreboard.h"

extern vex::brain Brain;

// Implementation

void Game::setScore(int score) {
    Game::score = score;
    Scoreboard::displayScore(score);

    if (score > Game::highScore) {
        Game::highScore = score;
        Scoreboard::displayHighScore(highScore);
    }
}

void Game::incrementScore() {
    setScore(Game::score + 1);
}

void Game::incrementScore(int amount) {
    setScore(Game::score + amount);
}

// Internal
