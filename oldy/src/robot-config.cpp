#include "vex.h"

using namespace vex;
using signature = vision::signature;
using code = vision::code;

// A global instance of brain used for printing to the V5 Brain screen
brain  Brain;

// VEXcode device constructors
motor leftback = motor(PORT3, ratio18_1, false);
motor rightback = motor(PORT10, ratio18_1, true);
motor leftfront = motor(PORT11, ratio18_1, false);
motor rightfront = motor(PORT20, ratio18_1, true);
motor goalarm = motor(PORT16, ratio18_1, false);
motor chain = motor(PORT6, ratio18_1, false);
motor lift = motor(PORT5, ratio18_1, false);
motor clamp = motor(PORT9, ratio18_1, false);
signature colour__BLUEGOAL = signature (1, -3065, -1867, -2466, 4821, 13153, 8987, 1.6, 0);
signature colour__YELLOWGOAL = signature (2, -1, 1297, 648, -3723, -1799, -2761, 1.8, 0);
signature colour__REDGOAL = signature (3, 3753, 9417, 6585, -961, 267, -347, 1.2, 0);
vision colour = vision (PORT18, 50, colour__BLUEGOAL, colour__YELLOWGOAL, colour__REDGOAL);
signature colourb__BLUEGOAL = signature (1, -3065, -1867, -2466, 4821, 13153, 8987, 1.6, 0);
signature colourb__YELLOWGOAL = signature (2, -1, 1297, 648, -3723, -1799, -2761, 1.8, 0);
signature colourb__REDGOAL = signature (3, 3753, 9417, 6585, -961, 267, -347, 1.2, 0);
vision colourb = vision (PORT8, 50, colourb__BLUEGOAL, colourb__YELLOWGOAL, colourb__REDGOAL);
inertial inertia = inertial(PORT15);


sonar distancer = sonar(Brain.ThreeWirePort.C);
pot potG = pot(Brain.ThreeWirePort.G);
encoder chainencoder = encoder(Brain.ThreeWirePort.A);
limit goallimit = limit(Brain.ThreeWirePort.F);




motor_group lefts(leftback, leftfront);
motor_group rights(rightback, rightfront);
motor_group allfour(leftback, leftfront, rightback, rightfront);

//motor_group allfour(lefts, rights);

controller remote = controller(primary);

// VEXcode generated functions
// define variable for remote controller enable/disable
bool RemoteControlCodeEnabled = true;

/**
 * Used to initialize code/tasks/devices added using tools in VEXcode Pro.
 * 
 * This should be called at the start of your int main function.
 */
void vexcodeInit( void ) {
  // nothing to initialize
}