/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       scoreboard.cpp                                                  */
/*    Author:       mitchellg2026                                             */
/*    Created:      Mon Mar 11 2024                                            */
/*    Description:  Implementation for Pop-A-Shot Game, Scoreboard            */
/*                                                                            */
/*----------------------------------------------------------------------------*/

#include "game.h"
#include "scoreboard.h"

extern vex::brain Brain;

// Internal

std::vector<std::vector<int>> numberMap(int n) {
    switch (n) {
        case 0: 
            return Internal::ZERO;
        case 1:
            return Internal::ONE;
        case 2:
            return Internal::TWO;
        case 3:
            return Internal::THREE;
        case 4:
            return Internal::FOUR;
        case 5:
            return Internal::FIVE;
        case 6:
            return Internal::SIX;
        case 7:
            return Internal::SEVEN;
        case 8:
            return Internal::EIGHT;
        case 9:
            return Internal::NINE;
    }

    return {};
}

void numbers(int hundreds, int tens, int ones) {
    Brain.Screen.clearScreen();

    std::vector<std::vector<int>> hundredsMap = numberMap(hundreds);
    std::vector<std::vector<int>> tensMap = numberMap(tens);
    std::vector<std::vector<int>> onesMap = numberMap(ones);

    for (int i = 0; i < hundredsMap.size(); i++) {
        std::vector<int> current = hundredsMap[i];
        Brain.Screen.drawRectangle(current[0], current[1], current[2], current[3], Internal::SCOREBOARD_COLOR);
    }

    for (int i = 0; i < tensMap.size(); i++) {
        std::vector<int> current = tensMap[i];
        Brain.Screen.drawRectangle(current[0] + Internal::TENS_SHIFT, current[1], current[2], current[3], Internal::SCOREBOARD_COLOR);
    }

    for (int i = 0; i < onesMap.size(); i++) {
        std::vector<int> current = onesMap[i];
        Brain.Screen.drawRectangle(current[0] + Internal::ONES_SHIFT, current[1], current[2], current[3], Internal::SCOREBOARD_COLOR);
    }
}

// Implementation

void Scoreboard::displayScore(int score) {
    int h = score / 100;
    int t = (score - h * 100) / 10;
    int o = score - h * 100 - t * 10;

    numbers(h, t, o);
}

void Scoreboard::displayHighScore(int score) {
    Brain.Screen.clearLine(1, vex::color::black);
    Brain.Screen.printAt(10, 10, "High Score: %d", score);
}