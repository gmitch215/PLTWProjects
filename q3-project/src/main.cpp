/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Author:       mitchellg2026                                             */
/*    Created:      Fri Mar 1 2024                                           */
/*    Description:  V5 project                                                */
/*                                                                            */
/*----------------------------------------------------------------------------*/
#include "vex.h"
#include "constants.h"

using namespace vex;

vex::brain      Brain;
vex::motor      Vertical       = vex::motor( vex::PORT2 );
vex::motor      Horizontal     = vex::motor( vex::PORT3 );
vex::distance   Distance       = vex::distance( vex::PORT1 );

int main() {
    
}
