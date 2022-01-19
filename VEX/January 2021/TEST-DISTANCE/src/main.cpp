/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Author:       VEX                                                       */
/*    Created:      Tue Oct 01 2019                                           */
/*    Description:  Detecting Distances                                       */
/*                  This program will use the Range Finder to stop a robot    */
/*                  300 mm away from a wall.                                  */
/*                                                                            */
/*----------------------------------------------------------------------------*/

// ---- START VEXCODE CONFIGURED DEVICES ----
// Robot Configuration:
// [Name]               [Type]        [Port(s)]
// distancer            sonar         C, D            
// ---- END VEXCODE CONFIGURED DEVICES ----

#include "vex.h"

using namespace vex;

int main() {
  // Initializing Robot Configuration. DO NOT REMOVE!
  vexcodeInit();
  // Wait 1 second before driving forward.
  wait(1, seconds);

  // The robot will stop driving when the Range Finder is less than 300 mm away
  // from an object.
while (true) {
    Brain.Screen.clearScreen();
    Brain.Screen.setCursor(1, 1); 
    Brain.Screen.print(distancer.distance(mm));

    wait(.1, sec);
  }
}
