/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       scoreboard.cpp                                            */
/*    Author:       mitchellg2026                                             */
/*    Created:      Mon Mar 11 2024                                           */
/*    Description:  Implementation for Pop-A-Shot Game, Scoreboard            */
/*                                                                            */
/*----------------------------------------------------------------------------*/

#include "game.h"
#include "scoreboard.h"

extern vex::brain Brain;

namespace Scoreboard {
    const vex::color SCOREBOARD_COLOR                         = vex::color(0xFFFFFF); // White
    const vex::color END_SCOREBOARD_COLOR                     = vex::color(0xFFD700); // Gold

    const int NUMBER_DIVIDER                                  = 50; // The distance between each number in pixels
    const int NUMBER_WIDTH                                    = 80; // The width of each number in pixels
    const int NUMBER_HEIGHT                                   = 100; // The height of each number in pixels
    const int BIG_PIXEL                                       = 10; // The square size of a "big pixel"
    const int NUMBER_DIVIDER_EXTRA                            = NUMBER_DIVIDER + BIG_PIXEL; // The distance between each number in pixels, plus the size of a "big pixel"
    
    // <X, Y, Width, Height>
    const std::vector<std::vector<int>> ZERO                  = {
        {BIG_PIXEL + 10, BIG_PIXEL, BIG_PIXEL, NUMBER_HEIGHT}, // Left
        {NUMBER_HEIGHT, BIG_PIXEL, BIG_PIXEL, NUMBER_HEIGHT}, // Right
        {BIG_PIXEL + 10, BIG_PIXEL, NUMBER_WIDTH, BIG_PIXEL}, // Top
        {BIG_PIXEL + 10, NUMBER_HEIGHT, NUMBER_WIDTH, BIG_PIXEL}, // Bottom
    };

    std::vector<std::vector<int>> ONE                   = {
        {NUMBER_DIVIDER_EXTRA, BIG_PIXEL, BIG_PIXEL, NUMBER_HEIGHT}
    };

    std::vector<std::vector<int>> TWO                   = {
        {BIG_PIXEL + 10, BIG_PIXEL, NUMBER_WIDTH, BIG_PIXEL}, // Top
        {NUMBER_HEIGHT, BIG_PIXEL, BIG_PIXEL, NUMBER_DIVIDER}, // Right
        {BIG_PIXEL + 10, NUMBER_DIVIDER_EXTRA, NUMBER_WIDTH, BIG_PIXEL}, // Middle
        {BIG_PIXEL + 10, NUMBER_DIVIDER_EXTRA, BIG_PIXEL, NUMBER_DIVIDER}, // Left
        {BIG_PIXEL + 10, NUMBER_HEIGHT, NUMBER_WIDTH, BIG_PIXEL} // Bottom
    };

    std::vector<std::vector<int>> THREE                 = {
        {BIG_PIXEL + 10, BIG_PIXEL, NUMBER_WIDTH, BIG_PIXEL}, // Top
        {NUMBER_HEIGHT, BIG_PIXEL, BIG_PIXEL, NUMBER_DIVIDER}, // Right
        {BIG_PIXEL + 10, NUMBER_DIVIDER_EXTRA, NUMBER_WIDTH, BIG_PIXEL}, // Middle
        {NUMBER_HEIGHT, NUMBER_DIVIDER_EXTRA, BIG_PIXEL, NUMBER_DIVIDER}, // Right
        {BIG_PIXEL + 10, NUMBER_HEIGHT, NUMBER_WIDTH, BIG_PIXEL} // Bottom
    };

    std::vector<std::vector<int>> FOUR                  = {
        {BIG_PIXEL + 10, BIG_PIXEL, BIG_PIXEL, NUMBER_DIVIDER}, // Left
        {NUMBER_HEIGHT, BIG_PIXEL, BIG_PIXEL, NUMBER_DIVIDER}, // Right
        {BIG_PIXEL + 10, NUMBER_DIVIDER_EXTRA, NUMBER_WIDTH, BIG_PIXEL}, // Middle
        {NUMBER_HEIGHT, NUMBER_DIVIDER_EXTRA, BIG_PIXEL, NUMBER_DIVIDER} // Right
    };

    std::vector<std::vector<int>> FIVE                  = {
        {BIG_PIXEL + 10, BIG_PIXEL, NUMBER_WIDTH, BIG_PIXEL}, // Top
        {BIG_PIXEL + 10, BIG_PIXEL, BIG_PIXEL, NUMBER_DIVIDER}, // Left
        {BIG_PIXEL + 10, NUMBER_DIVIDER_EXTRA, NUMBER_WIDTH, BIG_PIXEL}, // Middle
        {NUMBER_HEIGHT, NUMBER_DIVIDER_EXTRA, BIG_PIXEL, NUMBER_DIVIDER}, // Right
        {BIG_PIXEL + 10, NUMBER_HEIGHT, NUMBER_WIDTH, BIG_PIXEL} // Bottom
    };

    std::vector<std::vector<int>> SIX                   = {
        {BIG_PIXEL + 10, BIG_PIXEL, NUMBER_WIDTH, BIG_PIXEL}, // Top
        {BIG_PIXEL + 10, BIG_PIXEL, BIG_PIXEL, NUMBER_DIVIDER}, // Left
        {BIG_PIXEL + 10, NUMBER_DIVIDER_EXTRA, NUMBER_WIDTH, BIG_PIXEL}, // Middle
        {NUMBER_HEIGHT, NUMBER_DIVIDER_EXTRA, BIG_PIXEL, NUMBER_DIVIDER}, // Right
        {BIG_PIXEL + 10, NUMBER_HEIGHT, NUMBER_WIDTH, BIG_PIXEL}, // Bottom
        {BIG_PIXEL + 10, NUMBER_DIVIDER_EXTRA, BIG_PIXEL, NUMBER_DIVIDER} // Left
    };

    std::vector<std::vector<int>> SEVEN                 = {
        {BIG_PIXEL + 10, BIG_PIXEL, NUMBER_WIDTH, BIG_PIXEL}, // Top
        {NUMBER_HEIGHT, BIG_PIXEL, BIG_PIXEL, NUMBER_DIVIDER}, // Right
        {NUMBER_HEIGHT, NUMBER_DIVIDER_EXTRA, BIG_PIXEL, NUMBER_DIVIDER} // Right
    };

    std::vector<std::vector<int>> EIGHT                 = {
        {BIG_PIXEL + 10, BIG_PIXEL, NUMBER_WIDTH, BIG_PIXEL}, // Top
        {BIG_PIXEL + 10, BIG_PIXEL, BIG_PIXEL, NUMBER_DIVIDER}, // Left
        {NUMBER_HEIGHT, BIG_PIXEL, BIG_PIXEL, NUMBER_DIVIDER}, // Right
        {BIG_PIXEL + 10, NUMBER_DIVIDER_EXTRA, NUMBER_WIDTH, BIG_PIXEL}, // Middle
        {BIG_PIXEL + 10, NUMBER_DIVIDER_EXTRA, BIG_PIXEL, NUMBER_DIVIDER}, // Left
        {NUMBER_HEIGHT, NUMBER_DIVIDER_EXTRA, BIG_PIXEL, NUMBER_DIVIDER}, // Right
        {BIG_PIXEL + 10, NUMBER_HEIGHT, NUMBER_WIDTH, BIG_PIXEL} // Bottom
    };

    std::vector<std::vector<int>> NINE                  = {
        {BIG_PIXEL + 10, BIG_PIXEL, NUMBER_WIDTH, BIG_PIXEL}, // Top
        {BIG_PIXEL + 10, BIG_PIXEL, BIG_PIXEL, NUMBER_DIVIDER}, // Left
        {NUMBER_HEIGHT, BIG_PIXEL, BIG_PIXEL, NUMBER_DIVIDER}, // Right
        {BIG_PIXEL + 10, NUMBER_DIVIDER_EXTRA, NUMBER_WIDTH, BIG_PIXEL}, // Middle
        {NUMBER_HEIGHT, NUMBER_DIVIDER_EXTRA, BIG_PIXEL, NUMBER_DIVIDER}, // Right
        {BIG_PIXEL + 10, NUMBER_HEIGHT, NUMBER_WIDTH, BIG_PIXEL} // Bottom
    };

    const unsigned int TENS_SHIFT                             = NUMBER_WIDTH + NUMBER_DIVIDER;
    const unsigned int ONES_SHIFT                             = TENS_SHIFT * 2;
}

// Internal

/**
 * @brief Returns a 2D Array representing the pixel map of a number
 * Gets the pixel map of a number from 0-9.
 * @param n Integer between 0 and 9
 * @return std::vector<std::vector<int>> 2D Array representing the pixel map of a number 
 */
std::vector<std::vector<int>> numberMap(int n) {
    switch (n) {
        case 0: 
            return Scoreboard::ZERO;
        case 1:
            return Scoreboard::ONE;
        case 2:
            return Scoreboard::TWO;
        case 3:
            return Scoreboard::THREE;
        case 4:
            return Scoreboard::FOUR;
        case 5:
            return Scoreboard::FIVE;
        case 6:
            return Scoreboard::SIX;
        case 7:
            return Scoreboard::SEVEN;
        case 8:
            return Scoreboard::EIGHT;
        case 9:
            return Scoreboard::NINE;
    }

    return {};
}

// Implementation

/**
 * @brief Displays the score on the screen.
 * Displays an integer score on the Vex V5 Screen using the default scoreboard color.
 * @param score The score to display
 */
void Scoreboard::displayScore(int score) {
    int h = score / 100;
    int t = (score - h * 100) / 10;
    int o = score - h * 100 - t * 10;

    numbers(h, t, o, Scoreboard::SCOREBOARD_COLOR);
}

/**
 * @brief Internal Scoreboard Display Function
 * Displays the score on the screen.
 * @param hundreds The number to display in the hundreds place
 * @param tens The number to display in the tens place
 * @param ones The number to display in the ones place
 * @param color The color to display the numbers
 */
void Scoreboard::numbers(int hundreds, int tens, int ones, vex::color color) {
    Brain.Screen.clearScreen();

    std::vector<std::vector<int>> hundredsMap = numberMap(hundreds); // Pixel Map for Hundreds
    std::vector<std::vector<int>> tensMap = numberMap(tens); // Pixel Map for Tens
    std::vector<std::vector<int>> onesMap = numberMap(ones); // Pixel Map for Ones

    // Draws the numbers on the screen
    for (int i = 0; i < hundredsMap.size(); i++) {
        std::vector<int> current = hundredsMap[i];

        // <X, Y, Width, Height>, Color
        Brain.Screen.drawRectangle(current[0], current[1], current[2], current[3], color);
    }

    for (int i = 0; i < tensMap.size(); i++) {
        std::vector<int> current = tensMap[i];
        Brain.Screen.drawRectangle(current[0] + Scoreboard::TENS_SHIFT, current[1], current[2], current[3], color);
    }

    for (int i = 0; i < onesMap.size(); i++) {
        std::vector<int> current = onesMap[i];
        Brain.Screen.drawRectangle(current[0] + Scoreboard::ONES_SHIFT, current[1], current[2], current[3], color);
    }
}
/**
 * @brief Ends the game.
 * Ends the game on the scoreboard side, displaying the final score in the end scoreboard color.
 * @param score The final score to display
 */
void Scoreboard::end(int score) {
    int h = score / 100;
    int t = (score - h * 100) / 10;
    int o = score - h * 100 - t * 10;

    numbers(h, t, o, Scoreboard::END_SCOREBOARD_COLOR);
}