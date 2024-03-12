/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Author:       mitchellg2026                                             */
/*    Created:      Fri Mar 1 2024                                           */
/*    Description:  V5 project                                                */
/*                                                                            */
/*----------------------------------------------------------------------------*/
#include "vex.h"
#include "game.h"
#include "scoreboard.h"
#include "buttons.h"

using namespace vex;
using namespace Internal;
using namespace Game;

vex::brain      Brain;
vex::motor      Vertical       = vex::motor( vex::PORT2 );
vex::motor      Horizontal     = vex::motor( vex::PORT3 );
vex::distance   Distance       = vex::distance( vex::PORT1 );

int main() {
    vex::button_ui ui;
    ui.clear();

    auto changeDifficulty = [](int i, bool state, int id) -> bool {
        difficulty = id;
        return true;
    };

    ui.addButton( 10, 10, 100, 50, vex::color::green, "Easy", changeDifficulty, 0);
    ui.addButton( 10, 70, 100, 50, vex::color::yellow, "Medium", changeDifficulty, 1);
    ui.addButton( 10, 130, 100, 50, vex::color::red, "Hard", changeDifficulty, 2);

    ui.enable();
    ui.display();
}
