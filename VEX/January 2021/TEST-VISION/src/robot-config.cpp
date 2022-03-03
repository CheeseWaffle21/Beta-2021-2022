#include "vex.h"

using namespace vex;
using signature = vision::signature;
using code = vision::code;

// A global instance of brain used for printing to the V5 Brain screen
brain  Brain;

// VEXcode device constructors
/*vex-vision-config:begin*/
signature colour__BLUEGOAL = signature (1, -3441, -2785, -3113, 8975, 10355, 9665, 5.8, 0);
signature colour__YELLOWGOAL = signature (2, 2073, 4617, 3345, -4089, -3745, -3917, 8.4, 0);
signature colour__REDGOAL = signature (3, 8099, 8893, 8496, -1505, -949, -1227, 2.5, 0);
vision colour = vision (PORT18, 50, colour__BLUEGOAL, colour__YELLOWGOAL, colour__REDGOAL);
/*vex-vision-config:end*/
/*vex-vision-config:begin*/
signature colourb__BLUEGOAL = signature (1, -3441, -2785, -3113, 8975, 10355, 9665, 5.8, 0);
signature colourb__YELLOWGOAL = signature (2, 2073, 4617, 3345, -4089, -3745, -3917, 8.4, 0);
signature colourb__REDGOAL = signature (3, 8099, 8893, 8496, -1505, -949, -1227, 2.5, 0);
vision colourb = vision (PORT2, 50, colourb__BLUEGOAL, colourb__YELLOWGOAL, colourb__REDGOAL);

/*vex-vision-config:end*/

// VEXcode generated functions



/**
 * Used to initialize code/tasks/devices added using tools in VEXcode Pro.
 * 
 * This should be called at the start of your int main function.
 */
void vexcodeInit( void ) {
  // nothing to initialize
}