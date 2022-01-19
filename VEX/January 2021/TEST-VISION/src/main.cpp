/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Author:       VEX                                                       */
/*    Created:      Fri Sep 27 2019                                           */
/*    Description:  V5 project                                                */
/*                                                                            */
/*    This project will detect 3 different colored objects and display        */
/*    when each object is found on the V5 Brain's screen.                     */
/*                                                                            */
/*----------------------------------------------------------------------------*/

// ---- START VEXCODE CONFIGURED DEVICES ----
// Robot Configuration:
// [Name]               [Type]        [Port(s)]
// colour               vision        9               
// ---- END VEXCODE CONFIGURED DEVICES ----

#include "vex.h"

using namespace vex;

event checkRed = event();
event checkBlue = event();
event checkGreen = event();

void hasBlueCallback() {
  Brain.Screen.print("hello");
  while (true) {
    colour.takeSnapshot(colour__BLUEGOAL);
    Brain.Screen.clearScreen();
    Brain.Screen.setCursor(1, 1); 
    //Brain.Screen.print(colour.objectCount);    
    if (colour.largestObject.centerX > 100 && colour.largestObject.centerX < 120) {
      Brain.Screen.print("yes");
    }
    wait(.05, sec);
  }
  
}

int main() {
  // Initializing Robot Configuration. DO NOT REMOVE!
  vexcodeInit();
    Brain.Screen.print("hello");
  while (true) {
    colour.takeSnapshot(colour__BLUEGOAL);

    //Brain.Screen.print(colour.objectCount);    
    if (colour.largestObject.centerX > 100 && colour.largestObject.centerX < 120) {
      Brain.Screen.print("yes");
    }
    wait(.05, sec);
  }

}
