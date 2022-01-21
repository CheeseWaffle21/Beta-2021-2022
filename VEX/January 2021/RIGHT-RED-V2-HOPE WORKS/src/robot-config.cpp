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
motor goalarm = motor(PORT19, ratio18_1, false);
motor chain = motor(PORT6, ratio18_1, false);
motor lift = motor(PORT5, ratio18_1, false);
motor clamp = motor(PORT8, ratio18_1, false);
signature colour__BLUEGOAL = signature (1, -3441, -2785, -3113, 8975, 10355, 9665, 9.1, 0);
signature colour__GREENBOX = signature (2, -5767, -4965, -5366, -3803, -2861, -3332, 2.5, 0);
signature colour__REDGOAL = signature (3, 8099, 8893, 8496, -1505, -949, -1227, 10, 0);
vision colour = vision (PORT9, 50, colour__BLUEGOAL, colour__GREENBOX, colour__REDGOAL);

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