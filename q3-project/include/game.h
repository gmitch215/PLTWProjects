#include "vex.h"

#include <vector>

#ifndef Q3_GAME_H
#define Q3_GAME_H

namespace Game {

    int getDifficulty();

    std::vector<int> getDifficultyValues();

    void setDifficulty(int difficulty);

    void setScore(int score);

    void incrementScore();

    void incrementScore(int amount);

    // Game

    void loadGame(std::vector<vex::distance> Distances);

    void setTime(int time);
};

#endif