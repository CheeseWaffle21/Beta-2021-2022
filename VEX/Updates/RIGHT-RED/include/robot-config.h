using namespace vex;

extern brain Brain;

using signature = vision::signature;


// VEXcode devices
extern motor leftback;
extern motor rightback;
extern motor leftfront;
extern motor rightfront;
extern motor goalarm;
extern motor chain;
extern motor lift;
extern motor clamp;
extern controller remote;
extern motor_group lefts;
extern motor_group rights;
extern motor_group allfour;
extern signature colour__BLUEGOAL;
extern signature colour__GREENBOX;
extern signature colour__REDGOAL;
extern vision colour;
extern sonar distancer;
extern pot potG;
extern encoder chainencoder;
extern limit goallimit;




/**
 * Used to initialize code/tasks/devices added using tools in VEXcode Pro.
 * 
 * This should be called at the start of your int main function.
 */
void  vexcodeInit( void );