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
    const vex::color SCOREBOARD_COLOR                         = vex::color(0xFFFFFF);
    const vex::color END_SCOREBOARD_COLOR                     = vex::color(0xFFD700);

    const int NUMBER_DIVIDER                                  = 50;
    const int NUMBER_WIDTH                                    = 80;
    const int NUMBER_HEIGHT                                   = 100;
    const int BIG_PIXEL                                       = 10;
    const int NUMBER_DIVIDER_EXTRA                            = NUMBER_DIVIDER + BIG_PIXEL;
    
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

void Scoreboard::displayScore(int score) {
    int h = score / 100;
    int t = (score - h * 100) / 10;
    int o = score - h * 100 - t * 10;

    numbers(h, t, o, Scoreboard::SCOREBOARD_COLOR);
}

void Scoreboard::numbers(int hundreds, int tens, int ones, vex::color color) {
    Brain.Screen.clearScreen();

    std::vector<std::vector<int>> hundredsMap = numberMap(hundreds);
    std::vector<std::vector<int>> tensMap = numberMap(tens);
    std::vector<std::vector<int>> onesMap = numberMap(ones);

    for (int i = 0; i < hundredsMap.size(); i++) {
        std::vector<int> current = hundredsMap[i];
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

void Scoreboard::end(int score) {
    int h = score / 100;
    int t = (score - h * 100) / 10;
    int o = score - h * 100 - t * 10;

    numbers(h, t, o, Scoreboard::END_SCOREBOARD_COLOR);
}