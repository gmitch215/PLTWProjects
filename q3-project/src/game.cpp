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
    const int TIME_DELAY                   = 10;

    // <Time, Score Multiplier>
    const std::vector<int> EASY            = {60, 1};
    const std::vector<int> MEDIUM          = {45, 2};
    const std::vector<int> HARD            = {30, 3};

    const std::vector<std::vector<int>> DIFFICULTY = {EASY, MEDIUM, HARD};

    // Variables
    unsigned long score                    = 0;
    unsigned int difficulty                = 0;
}

// Implementation

/**
 * @brief Gets the internal difficulty value of the game
 * Fetches the internal difficulty value of the game. 0 is easy, 1 is medium, and 2 is hard.
 * @return int The difficulty value of the game
 */
int Game::getDifficulty() {
    return Game::difficulty;
}

/**
 * @brief Gets the difficulty array containing information based on the current difficulty.
 * The difficulty array contianing info based on the current difficulty. This contains things such as the time limit and the score multiplier.
 * @return std::vector<int> The difficulty array
 */
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

/**
 * @brief Sets the current difficulty.
 * Sets the internal difficulty value.
 * @param difficulty Value of the difficulty
 */
void Game::setDifficulty(int difficulty) {
    Game::difficulty = difficulty;
}

/**
 * @brief Sets the current score.
 * Updates the internal score value and displays the score on the screen.
 * @param score Score to display
 */
void Game::setScore(int score) {
    Game::score = score;
    Scoreboard::displayScore(score);
}

/**
 * @brief Increments the score by 1.
 * Increments the score by 1 and displays the score on the screen.
 */
void Game::incrementScore() {
    setScore(Game::score + 1);
}

/**
 * @brief Increments the score by a specified amount.
 * Increments the score by a specified amount and displays the score on the screen.
 * @param amount Amount to increment the score by
 */
void Game::incrementScore(int amount) {
    setScore(Game::score + amount);
}

/**
 * @brief Sets the time on the screen.
 * Sets the displayed time on the screen.
 * @param time Time to display, in seconds
 */
void Game::setTime(int time) {
    double t = time / TIME_DELAY;
    Brain.Screen.printAt(10, 220, "Time: %2.3f", t);
}

/**
 * @brief Loads the game.
 * Performs the loading game operation. This inputs the distance sensors connected to the brain used to determine when to increment the score.
 * @param Sides Vector of distance sensors on the sides
 * @param Back Distance sensor on the back
 */
void Game::loadGame(std::vector<vex::distance> Sides, vex::distance Back) {
    Brain.Screen.clearScreen();

    std::vector<int> diff = Game::getDifficultyValues();
    int time = diff[0] * TIME_DELAY;
    int score = diff[1];

    Game::setScore(0);
    Game::setTime(time);

    // Start Game
    for (int i = time; i >= 0; i--) { // Performs a loop until the time runs out
        Game::setTime(i); // Displays the time on the screen
        
        if (time == 0)
            break;

        for (vex::distance &Distance : Sides)
            if (Distance.objectDistance(vex::inches) < 3) { // When recognized, increment the score
                Game::incrementScore(score);
                Game::setTime(i);
            }
        
        if (Back.objectDistance(vex::inches) < 8) { // Similar to above, but for the back sensor
            Game::incrementScore(score);
            Game::setTime(i);
        }

        vex::wait(1000 / TIME_DELAY, vex::msec); // Time down by 1s, proportional to TIME_DELAY
    }

    // End Game

    Scoreboard::end(Game::score);
}