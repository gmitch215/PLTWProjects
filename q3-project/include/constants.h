#ifndef Q3_CONSTANTS_H
#define Q3_CONSTANTS_H

#include <unordered_map>

namespace Game {
    // Constants
    const unsigned int EASY                = 1;
    const unsigned int EASY_TIME           = 60;

    const unsigned int MEDIUM              = 2;
    const unsigned int MEDIUM_TIME         = 45;

    const unsigned int HARD                = 3;
    const unsigned int HARD_TIME           = 30;

    // Variables
    unsigned long score                    = 0;
}

namespace Internal {

    const int NUMBER_DIVIDER                                  = 50;
    const int NUMBER_WIDTH                                    = 80;
    const int NUMBER_HEIGHT                                   = 100;
    const int BIG_PIXEL                                       = 10;
    const int LONG_PIXEL                                      = BIG_PIXEL * 2;
    
    // <X, Y, Width, Height>
    const std::vector<std::vector<int>> ZERO                  = {
        {LONG_PIXEL, BIG_PIXEL, BIG_PIXEL, NUMBER_HEIGHT}, // Left
        {100, BIG_PIXEL, BIG_PIXEL, NUMBER_HEIGHT}, // Right
        {LONG_PIXEL, BIG_PIXEL, NUMBER_WIDTH, LONG_PIXEL}, // Top
        {LONG_PIXEL, 100, NUMBER_WIDTH, LONG_PIXEL}, // Bottom
    };

    std::vector<std::vector<int>> ONE                   = {
        {60, BIG_PIXEL, BIG_PIXEL, 100}
    };

    std::vector<std::vector<int>> TWO                   = {
        {LONG_PIXEL, BIG_PIXEL, NUMBER_WIDTH, BIG_PIXEL}, // Top
        {100, BIG_PIXEL, BIG_PIXEL, NUMBER_DIVIDER}, // Right
        {LONG_PIXEL, 60, NUMBER_WIDTH, BIG_PIXEL}, // Middle
        {LONG_PIXEL, 60, BIG_PIXEL, NUMBER_DIVIDER}, // Left
        {LONG_PIXEL, 100, NUMBER_WIDTH, BIG_PIXEL} // Bottom
    };

    std::vector<std::vector<int>> THREE                 = {
        {LONG_PIXEL, BIG_PIXEL, NUMBER_WIDTH, BIG_PIXEL}, // Top
        {100, BIG_PIXEL, BIG_PIXEL, NUMBER_DIVIDER}, // Right
        {LONG_PIXEL, 60, NUMBER_WIDTH, BIG_PIXEL}, // Middle
        {100, 60, BIG_PIXEL, NUMBER_DIVIDER}, // Right
        {LONG_PIXEL, 100, NUMBER_WIDTH, BIG_PIXEL} // Bottom
    };

    std::vector<std::vector<int>> FOUR                  = {
        {LONG_PIXEL, BIG_PIXEL, BIG_PIXEL, NUMBER_DIVIDER}, // Left
        {100, BIG_PIXEL, BIG_PIXEL, NUMBER_DIVIDER}, // Right
        {LONG_PIXEL, 60, NUMBER_WIDTH, BIG_PIXEL}, // Middle
        {100, 60, BIG_PIXEL, NUMBER_DIVIDER} // Right
    };

    std::vector<std::vector<int>> FIVE                  = {
        {LONG_PIXEL, BIG_PIXEL, NUMBER_WIDTH, BIG_PIXEL}, // Top
        {LONG_PIXEL, BIG_PIXEL, BIG_PIXEL, NUMBER_DIVIDER}, // Left
        {LONG_PIXEL, 60, NUMBER_WIDTH, BIG_PIXEL}, // Middle
        {100, 60, BIG_PIXEL, NUMBER_DIVIDER}, // Right
        {LONG_PIXEL, 100, NUMBER_WIDTH, BIG_PIXEL} // Bottom
    };

    std::vector<std::vector<int>> SIX                   = {
        {LONG_PIXEL, BIG_PIXEL, NUMBER_WIDTH, BIG_PIXEL}, // Top
        {LONG_PIXEL, BIG_PIXEL, BIG_PIXEL, NUMBER_DIVIDER}, // Left
        {LONG_PIXEL, 60, NUMBER_WIDTH, BIG_PIXEL}, // Middle
        {100, 60, BIG_PIXEL, NUMBER_DIVIDER}, // Right
        {LONG_PIXEL, 100, NUMBER_WIDTH, BIG_PIXEL}, // Bottom
        {LONG_PIXEL, 60, BIG_PIXEL, NUMBER_DIVIDER} // Left
    };

    std::vector<std::vector<int>> SEVEN                 = {
        {LONG_PIXEL, BIG_PIXEL, NUMBER_WIDTH, BIG_PIXEL}, // Top
        {100, BIG_PIXEL, BIG_PIXEL, NUMBER_DIVIDER}, // Right
        {100, 60, BIG_PIXEL, NUMBER_DIVIDER} // Right
    };

    std::vector<std::vector<int>> EIGHT                 = {
        {LONG_PIXEL, BIG_PIXEL, NUMBER_WIDTH, BIG_PIXEL}, // Top
        {LONG_PIXEL, BIG_PIXEL, BIG_PIXEL, NUMBER_DIVIDER}, // Left
        {100, BIG_PIXEL, BIG_PIXEL, NUMBER_DIVIDER}, // Right
        {LONG_PIXEL, 60, NUMBER_WIDTH, BIG_PIXEL}, // Middle
        {LONG_PIXEL, 60, BIG_PIXEL, NUMBER_DIVIDER}, // Left
        {100, 60, BIG_PIXEL, NUMBER_DIVIDER}, // Right
        {LONG_PIXEL, 100, NUMBER_WIDTH, BIG_PIXEL} // Bottom
    };

    std::vector<std::vector<int>> NINE                  = {
        {LONG_PIXEL, BIG_PIXEL, NUMBER_WIDTH, BIG_PIXEL}, // Top
        {LONG_PIXEL, BIG_PIXEL, BIG_PIXEL, NUMBER_DIVIDER}, // Left
        {100, BIG_PIXEL, BIG_PIXEL, NUMBER_DIVIDER}, // Right
        {LONG_PIXEL, 60, NUMBER_WIDTH, BIG_PIXEL}, // Middle
        {100, 60, BIG_PIXEL, NUMBER_DIVIDER}, // Right
        {LONG_PIXEL, 100, NUMBER_WIDTH, BIG_PIXEL} // Bottom
    };

    const unsigned int TENS_SHIFT                             = NUMBER_WIDTH + NUMBER_DIVIDER;
    const unsigned int ONES_SHIFT                             = TENS_SHIFT * 2;

};

#endif