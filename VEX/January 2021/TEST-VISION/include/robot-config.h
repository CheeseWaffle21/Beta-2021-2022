using namespace vex;

extern brain Brain;

using signature = vision::signature;

// VEXcode devices
extern signature colour__BLUEGOAL;
extern signature colour__YELLOWGOAL;
extern signature colour__REDGOAL;
extern signature colour__SIG_4;
extern signature colour__SIG_5;
extern signature colour__SIG_6;
extern signature colour__SIG_7;
extern vision colour;
extern signature colourb__BLUEGOAL;
extern signature colourb__YELLOWGOAL;
extern signature colourb__REDGOAL;
extern signature colourb__SIG_4;
extern signature colourb__SIG_5;
extern signature colourb__SIG_6;
extern signature colourb__SIG_7;
extern vision colourb;

/**
 * Used to initialize code/tasks/devices added using tools in VEXcode Pro.
 * 
 * This should be called at the start of your int main function.
 */
void  vexcodeInit( void );