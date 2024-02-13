/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Author:       mitchellg2026                                                  */
/*    Created:      Tue Feb 13 2024                                           */
/*    Description:  V5 project                                                */
/*                                                                            */
/*----------------------------------------------------------------------------*/
#include "vex.h"
#include "tetris.h"

using namespace vex;

vex::brain  Brain;

int main() {
    tetrisInit();

    while(1) {
        tetrisStart();

        this_thread::sleep_for(1000);
    }
}
