using namespace vex;

extern brain Brain;

// VEXcode devices
extern motor frontleft;
extern motor backleft;
extern motor backright;
extern motor frontright;
extern controller Controller1;
extern motor boostright;
extern motor boostleft;
extern digital_out clamp;
extern inertial inertia;
extern encoder righttracker;
extern encoder lefttracker;
extern motor arm;
extern triport expander;
extern digital_out tilter;
extern sonar ultrasonic;
extern motor chain;
extern limit LimitSwitchC;

/**
 * Used to initialize code/tasks/devices added using tools in VEXcode Pro.
 * 
 * This should be called at the start of your int main function.
 */
void  vexcodeInit( void );