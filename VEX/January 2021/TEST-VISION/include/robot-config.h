using namespace vex;

extern brain Brain;

using signature = vision::signature;

// VEXcode devices
extern signature colour__BLUEGOAL;
extern signature colour__GREENBOX;
extern signature colour__REDBOX;
extern signature colour__SIG_4;
extern signature colour__SIG_5;
extern signature colour__SIG_6;
extern signature colour__SIG_7;
extern vision colour;

/**
 * Used to initialize code/tasks/devices added using tools in VEXcode Pro.
 * 
 * This should be called at the start of your int main function.
 */
void  vexcodeInit( void );