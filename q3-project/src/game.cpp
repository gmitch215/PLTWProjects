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

namespace Game {
    // Time, Score Multiplier
    const int TIME_DELAY                   = 10;

    const std::vector<int> EASY            = {60, 1};
    const std::vector<int> MEDIUM          = {45, 2};
    const std::vector<int> HARD            = {30, 3};

    const std::vector<std::vector<int>> DIFFICULTY = {EASY, MEDIUM, HARD};

    // Variables
    unsigned long score                    = 0;
    unsigned int difficulty                = 0;
}

// Implementation

int Game::getDifficulty() {
    return Game::difficulty;
}

std::vector<int> Game::getDifficultyValues() {
    switch (Game::difficulty) {
        case 0:
            return Game::EASY;
        case 1:
            return Game::MEDIUM;
        case 2:
            return Game::HARD;
        default:
            return Game::EASY;
    }
}

void Game::setDifficulty(int difficulty) {
    Game::difficulty = difficulty;
}

void Game::setScore(int score) {
    Game::score = score;
    Scoreboard::displayScore(score);
}

void Game::incrementScore() {
    setScore(Game::score + 1);
}

void Game::incrementScore(int amount) {
    setScore(Game::score + amount);
}

void Game::setTime(int time) {
    double t = time / TIME_DELAY;
    Brain.Screen.printAt(10, 220, "Time: %2.3f", t);
}

void Game::loadGame(std::vector<vex::distance> Distances) {
    Brain.Screen.clearScreen();

    std::vector<int> diff = Game::getDifficultyValues();
    int time = diff[0] * TIME_DELAY;
    int score = diff[1];

    Game::setScore(0);
    Game::setTime(time);

    // Start Game
    for (int i = time; i >= 0; i--) {
        Game::setTime(i);
        
        if (time == 0)
            break;

        for (vex::distance &Distance : Distances)
            if (Distance.objectDistance(vex::inches) < 5) {
                Game::incrementScore(score);
                Game::setTime(i);
            }
        

        vex::wait(1000 / TIME_DELAY, vex::msec);
    }

    // End Game

    Scoreboard::end(Game::score);
}