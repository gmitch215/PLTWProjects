#ifndef Q3_SCOREBOARD_H
#define Q3_SCOREBOARD_H

#include "vex.h"

namespace Scoreboard {

    void displayScore(int score);

    void numbers(int hundreds, int tens, int ones, vex::color color);

    void end(int score);

}

#endif