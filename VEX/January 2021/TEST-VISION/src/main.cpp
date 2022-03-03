// ---- START VEXCODE CONFIGURED DEVICES ----
// Robot Configuration:
// [Name]               [Type]        [Port(s)]
// colour               vision        9               
// colourb              vision        2               
// ---- END VEXCODE CONFIGURED DEVICES ----
// ---- START VEXCODE CONFIGURED DEVICES ----
// Robot Configuration:
// [Name]               [Type]        [Port(s)]
// colour               vision        9               
// colourb              vision        1               
// ---- END VEXCODE CONFIGURED DEVICES ----
// ---- START VEXCODE CONFIGURED DEVICES ----
// Robot Configuration:
// [Name]               [Type]        [Port(s)]
// colour               vision        9               
// ---- END VEXCODE CONFIGURED DEVICES ----
// ---- START VEXCODE CONFIGURED DEVICES ----
// Robot Configuration:
// [Name]               [Type]        [Port(s)]
// colour               vision        9               
// ---- END VEXCODE CONFIGURED DEVICES ----
// ---- START VEXCODE CONFIGURED DEVICES ----
// Robot Configuration:
// [Name]               [Type]        [Port(s)]
// colour               vision        9               
// ---- END VEXCODE CONFIGURED DEVICES ----
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
  while (true) {
    Brain.Screen.clearScreen();

    colour.takeSnapshot(colour__REDGOAL);
    Brain.Screen.setCursor(1, 1); 
    Brain.Screen.print("Front Reds are %d, largest center X at %d", colour.objectCount, colour.largestObject.centerX);

    colour.takeSnapshot(colour__BLUEGOAL);
    Brain.Screen.setCursor(2, 1); 
    Brain.Screen.print("Front Blues are %d, largest center X at %d", colour.objectCount, colour.largestObject.centerX);

    colour.takeSnapshot(colour__YELLOWGOAL);
    Brain.Screen.setCursor(3, 1); 
    Brain.Screen.print("Front Yellows are %d, largest center X at %d", colour.objectCount, colour.largestObject.centerX);

    colourb.takeSnapshot(colourb__REDGOAL);
    Brain.Screen.setCursor(4, 1); 
    Brain.Screen.print("Back Reds are %d, largest center X at %d", colourb.objectCount, colourb.largestObject.centerX);

    colourb.takeSnapshot(colourb__BLUEGOAL);
    Brain.Screen.setCursor(5, 1); 
    Brain.Screen.print("Back Blues are %d, largest center X at %d", colourb.objectCount, colourb.largestObject.centerX);

    colourb.takeSnapshot(colourb__YELLOWGOAL);
    Brain.Screen.setCursor(6, 1); 
    Brain.Screen.print("Back Yellows are %d, largest center X at %d", colourb.objectCount, colourb.largestObject.centerX);

    wait(.5, seconds);
  }

}
