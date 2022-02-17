/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Author:       VEX                                                       */
/*    Created:      Thu Sep 26 2019                                           */
/*    Description:  Competition Template                                      */
/*                                                                            */
/*----------------------------------------------------------------------------*/

// ---- START VEXCODE CONFIGURED DEVICES ----
// Robot Configuration:
// [Name]               [Type]        [Port(s)]
// leftback             motor         11              
// rightback            motor         20              
// leftfront            motor         1               
// rightfront           motor         10              
// goalarm              motor         19              
// chain                motor         6               
// lift                 motor         5               
// clamp                motor         8               
// remote          controller                    
// ---- END VEXCODE CONFIGURED DEVICES ----

#include "vex.h"
#include <cmath>
#include "math.h"
#define pi 3.14159265

using namespace vex;

// A global instance of competition
competition Competition;

// define your global instances of motors and other devices here

/*---------------------------------------------------------------------------*/
/*                          Pre-Autonomous Functions                         */
/*                                                                           */
/*  You may want to perform some actions before the competition starts.      */
/*  Do them in the following function.  You must return from this function   */
/*  or the autonomous and usercontrol tasks will not be started.  This       */
/*  function is only called once after the V5 has been powered on and        */
/*  not every time that the robot is disabled.                               */
/*---------------------------------------------------------------------------*/

void pre_auton(void) {
  // Initializing Robot Configuration. DO NOT REMOVE!
  vexcodeInit();

  // All activities that occur before the competition starts
  // Example: clearing encoders, setting servo positions, ...
}

/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                              Autonomous Task                              */
/*                                                                           */
/*  This task is used to control your robot during the autonomous phase of   */
/*  a VEX Competition.                                                       */
/*                                                                           */
/*  You must modify the code to add your own robot specific commands here.   */
/*---------------------------------------------------------------------------*/
int total;
int fwdconversion = 28; //28 degrees travels 1 inch
int strafeconversion = 62; //62 degrees strafes 1 inch
double trnconversion = 17.5; //17.5 degrees turns some amount of degrees idk

//Resets drivetrain
void reset () {
  leftback.resetPosition();
  rightback.resetPosition();
  rightfront.resetPosition();
  leftfront.resetPosition();
  return;
}

//stops drivetrain
void end () {
  leftback.stop();
  rightback.stop();
  rightfront.stop();
  leftfront.stop();
  return;
}

void kill () {
  leftback.stop(hold);
  rightback.stop(hold);
  rightfront.stop(hold);
  leftfront.stop(hold);
  return;
}

//grab function false is clamp down, true is let go
void grab (bool up) {
  clamp.resetPosition();
  if (up == false){
    clamp.setVelocity(100, percent);
    clamp.rotateFor(-360, degrees, false);   
  } else {
    clamp.setVelocity(100, percent);
    clamp.rotateFor(360, degrees, false);   
  }
  clamp.setBrake(hold);
}

//False is up, true is down.
void lifty (bool up) {
  lift.resetPosition();
  if (up == false){
    lift.setVelocity(100, percent);
    lift.rotateFor(1200, degrees, false);
  } 
  else {
    lift.setVelocity(100, percent);
    lift.rotateFor(-1200, degrees, false);
  }
}
//Takes average of all drivetrain motors' rotational values
//Used in situations going straight foward or backward
int fwdaverage () {
  total = ((leftback.position(degrees) + rightback.position(degrees) + rightfront.position(degrees) + leftfront.position(degrees)) / 4);
  Brain.Screen.clearScreen();
  Brain.Screen.setCursor(1, 1);
  Brain.Screen.print(total);
  return total;
}

//
int saverage () {
  total = ((-leftback.position(degrees) + rightback.position(degrees) + -rightfront.position(degrees) + leftfront.position(degrees)) / 4);
  Brain.Screen.clearScreen();
  Brain.Screen.setCursor(1, 1);
  Brain.Screen.print(total);
  return total;
}



void front (bool way, double number, int speed) {
  reset();
  leftback.setVelocity(speed, percent);
  rightback.setVelocity(speed, percent);
  rightfront.setVelocity(speed, percent);
  leftfront.setVelocity(speed, percent);
  if (way == true) {

    double target = number * fwdconversion;
    leftback.spin(forward);
    rightback.spin(forward);
    leftfront.spin(forward);
    rightfront.spin(forward);
    int current;
    while (true) {
      current = fwdaverage();
      if (current >= target) {
        break;
      }
    }
    end();

  } else if (way == false) {

    double target = number * -fwdconversion;
    leftback.spin(reverse);
    rightback.spin(reverse);
    leftfront.spin(reverse);
    rightfront.spin(reverse);
    int current;
    while (true) {
      current = fwdaverage();
      if (current <= target) {
        break;
      }
    }
    end();

  }
  wait(.5, seconds);
  return;
}

void strafe (bool way, double number, int speed) {
  reset();
  leftback.setVelocity(speed, percent);
  rightback.setVelocity(speed, percent);
  rightfront.setVelocity(speed, percent);
  leftfront.setVelocity(speed, percent);
  if (way == true) {

    double target = number * strafeconversion;
    leftback.spin(reverse);
    rightback.spin(forward);
    leftfront.spin(forward);
    rightfront.spin(reverse);
    int current;
    while (true) {
      current = saverage();
      if (current >= target) {
        break;
      }
    }
    end();

  } else if (way == false) {

    double target = number * -strafeconversion;
    leftback.spin(forward);
    rightback.spin(reverse);
    leftfront.spin(reverse);
    rightfront.spin(forward);
    int current;
    while (true) {
      current = saverage();
      if (current <= target) {
        break;
      }
    }
    end();

  }
  wait(.5, seconds);
  return;
}

void frontarm (bool up) {
  goalarm.setVelocity(100, percent);
  if (up == true) { 
    goalarm.spin(forward);
    while (true) {
      Brain.Screen.clearScreen();
      Brain.Screen.setCursor(1, 1);
      Brain.Screen.print(potG.angle(degrees));
      if (potG.angle(degrees) <= 30) {
        break;
      }
      wait(.05, sec);
    }   
  } else if (up == false) {
    goalarm.spin(reverse);
    while (true) {
      Brain.Screen.clearScreen();
      Brain.Screen.setCursor(1, 1);
      Brain.Screen.print(potG.angle(degrees));
      if (potG.angle(degrees) >= 93) {
        break;
      }
      wait(.05, sec);
    }   
  }
  goalarm.stop();
  goalarm.setBrake(hold); 
}

int taverage () {
  total = ((leftback.position(degrees) + -rightback.position(degrees) + -rightfront.position(degrees) + leftfront.position(degrees)) / 4);
  Brain.Screen.clearScreen();
  Brain.Screen.setCursor(1, 1);
  Brain.Screen.print(total);
  return total;
}

void rotate (bool way, double number, int speed) {
  reset();
  leftback.setVelocity(speed, percent);
  rightback.setVelocity(speed, percent);
  rightfront.setVelocity(speed, percent);
  leftfront.setVelocity(speed, percent);
  if (way == true) {

    double target = number * trnconversion;
    leftback.spin(forward);
    rightback.spin(reverse);
    leftfront.spin(forward);
    rightfront.spin(reverse);
    int current;
    while (true) {
      current = taverage();
      if (current >= target) {
        break;
      }
    }
    end();

  } else if (way == false) {

    double target = number * -trnconversion;
    leftback.spin(reverse);
    rightback.spin(forward);
    leftfront.spin(reverse);
    rightfront.spin(forward);
    int current;
    while (true) {
      current = taverage();
      if (current <= target) {
        break;
      }
    }
    end();

  }
  wait(.5, seconds);
  return;
}

void rotate2 (bool way, int target, int speed) {
  leftback.setVelocity(speed, percent);
  rightback.setVelocity(speed, percent);
  rightfront.setVelocity(speed, percent);
  leftfront.setVelocity(speed, percent);
  
  int hdegrees;

  if (way == true) {
    hdegrees = inertia.rotation(degrees) + target;
    leftback.spin(forward);
    rightback.spin(reverse);
    leftfront.spin(forward);
    rightfront.spin(reverse);
    waitUntil(inertia.rotation(degrees) >= hdegrees);
    end();

  } else if (way == false) {
    hdegrees = inertia.rotation(degrees) - target;
    leftback.spin(reverse);
    rightback.spin(forward);
    leftfront.spin(reverse);
    rightfront.spin(forward);
    waitUntil(inertia.rotation(degrees) <= hdegrees);
    end();
  }
  
}

void rotate3 (bool way, int head, int speed){
  leftback.setVelocity(speed, percent);
  rightback.setVelocity(speed, percent);
  rightfront.setVelocity(speed, percent);
  leftfront.setVelocity(speed, percent);
  
  

  if (way==false) {
    leftback.spin(reverse);
    rightback.spin(forward);
    leftfront.spin(reverse);
    rightfront.spin(forward);

     waitUntil(sin((inertia.rotation(degrees)*pi/180)) <= 0.1);
    kill();
  }
}

void forwardtillbump (int speed) {
  leftback.setVelocity(speed, percent);
  rightback.setVelocity(speed, percent);
  rightfront.setVelocity(speed, percent);
  leftfront.setVelocity(speed, percent);

  leftback.spin(forward);
  rightback.spin(forward);
  leftfront.spin(forward);
  rightfront.spin(forward);

  waitUntil(goallimit.pressing());

  end();
}

void moveto (double x, double y, double turn) {
  
  double targetangle = atan2(y,x);
  
  double magnitude = sqrt(x*x + y*y) / 100;
  
  double FrBl = sin(targetangle - pi/4) * magnitude;
  double FlBr = sin(targetangle + pi/4) * magnitude;
  
  
  
  
  
  
  
}

//    fwd-back:
//    28 degrees = 1 inch
//
//    strafe:
//    62 degrees = 1 inch
void autonomous(void) {
  // ..........................................................................
  // Insert autonomous user code here.
  // ..........................................................................
  /*strafe (false, 18, 50);
  frontarm (false);
  front (true, 18, 30);
  frontarm (true);
  strafe (true, 18, 30);
  chain.spin(forward);
  for (int i = 1; i <= 4; i++) {
    front (false, 7, 70);
    wait (2, seconds);
    front (true, 7, 70);
  }
  rotate (true, 30, 50);
  */
  

  /*
  front (true, 90, 30);
  

  strafe (false, 11.7, 30);
  front (false, 50, 30);

  strafe (false, 17.7, 30);
  front (true, 50, 30);
  //front (false, 3, 30);

  strafe (false, 25, 30);
  front (false, 50, 30);

  front (true, 90, 30);
  strafe (false, 10, 30);
  front (false, 90, 30);
  */

  //Initialize inertial sensor
  inertia.startCalibration();
  wait(2, seconds);

  //Grab goal in front with front arm
  frontarm(false);
  forwardtillbump(50);
  frontarm(true);

  //potential for using match load rings here

  //Go back a bit and strafe a bit left
  front(false, 5, 50);
  strafe(false, 5, 50);


  rotate2(true, 85, 50); //96.1417757 degrees precisely
  
  //inertial move till collision or move for certian rotations guarantee meeting of the yellow mobile goal
  front(false, 54.2112, 50);
  
  
  //Transfer grab function to here
  grab(false);

  wait(1, seconds);
  
  //turn towards lower platform
  rotate2(true, 10, 50);

  //move till collision with the platform?

  //move forward 57.378 inches
  front(false, 35, 30);

  /*maybe rotate perpendicularly to goal:
  rotate2(false, -6, 50);
  */

  //lift the lift
  lifty(false);
  wait(1, seconds);

  //come to the platform
  front(false, 15, 50);

  //turn back
  rotate2(false, 20, 50);

  //strafe to level platform
  //strafe(true, 5, 50);*****

  //grab lets go of goal to balance the goal
  grab(true);


  rotate3(false, 0, 40);

  wait(1, seconds);

  lifty(true);

  front(false, 48, 50);

  grab(false);

  

/*
  //move backwards
  front(true, 12, 50);
  
  front(true, 3, 50);
  rotate2(false, 90, 50);
  frontarm(false);
  
  //move forwards till alliance goal
  front(false, 28, 70);

  //grab the alliance goal with clamp
  grab(true);

  //turn clockwise to face tall yellow
  //push tall yellow to the opposite corner
  



*/
}

/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                              User Control Task                            */
/*                                                                           */
/*  This task is used to control your robot during the user control phase of */
/*  a VEX Competition.                                                       */
/*                                                                           */
/*  You must modify the code to add your own robot specific commands here.   */
/*---------------------------------------------------------------------------*/
void deploy () {
  Brain.Screen.print("Event has occurred");
  chainencoder.setPosition(0, degrees);
  chain.spin(reverse);
  if (chainencoder.position(degrees) <= -180) {
    chain.stop();
  }
  /*      while (true) {
    Brain.Screen.clearScreen();
    Brain.Screen.setCursor(1, 1);
    Brain.Screen.print(chainencoder.position(degrees));
    if (chainencoder.position(degrees) >= 180) {
      break;
    }
    wait(.05, sec);
  }
  chain.stop();*/
  return;
}

void usercontrol(void) {
  // User control code here, inside the loop
int front;
int rotate;
int strafe;


int threshold = 10;
int backthreshold = -10;

bool goalarmstopped = true;
bool chainstopped = true;
bool liftstopped = true;
bool clampstopped = true;

bool toggleEnabled = false; 
bool buttonPressed = false; 

bool toggleEnabledb = false; 
bool buttonPressedb = false; 




goalarm.setVelocity(100, percent);
  chain.setVelocity(100, percent);
  lift.setVelocity(100, percent);
  clamp.setVelocity(100, percent);


  while (1) {
    // This is the main execution loop for the user control program.
    // Each time through the loop your program should update motor + servo
    // values based on feedback from the joysticks.

    // ........................................................................
    // Insert user code here. This is where you use the joystick values to
    // update your motors, etc.
    // ........................................................................
    front = remote.Axis3.position();
    rotate = remote.Axis4.position();
    strafe = remote.Axis1.position();

    if (front <= threshold && front >= backthreshold) {
      front = 0;
    }

    if (rotate <= threshold && rotate >= backthreshold) {
      rotate = 0;
    }

    if (strafe <= threshold && strafe >= backthreshold) {
      strafe = 0;
    }

    leftback.setVelocity(front + rotate - strafe, percent);
    leftback.spin(forward);

    rightback.setVelocity(front - rotate + strafe, percent);
    rightback.spin(forward);

    leftfront.setVelocity(front + rotate + strafe, percent);
    leftfront.spin(forward);

    rightfront.setVelocity(front - rotate - strafe, percent);
    rightfront.spin(forward);



    if (remote.ButtonL1.pressing()) {
      /*if (!up) {
        goalarm.spinFor(forward, 440, degrees);
        goalarm.setBrake(hold);
        up = true;
      }*/
     goalarm.spin(forward);
     goalarmstopped = false;
    } else if (remote.ButtonL2.pressing()) {
      /*if (up) {
        goalarm.spinFor(reverse, 440, degrees);
        goalarm.setBrake(hold);
        up = false;
      }*/
     goalarm.spin(reverse);
     goalarmstopped = false;
    } else if (!goalarmstopped) {
     goalarm.stop();
     goalarmstopped = true;
    }


    bool buttonX = remote.ButtonX.pressing();
    if (buttonX && !buttonPressed){
      buttonPressed = true; 
      toggleEnabled = !toggleEnabled;
    }
    else if (!buttonX) buttonPressed = false;

    if(toggleEnabled){
      chain.spin(forward);
    } else if (remote.ButtonB.pressing()) {
      chain.spin(reverse);
    }
    else{
      chain.stop();
    }

    /*bool buttonB = remote.ButtonB.pressing();
    if (buttonB && !buttonPressedb){
      buttonPressedb = true; 
      toggleEnabledb = !toggleEnabledb;
    }
    else if (!buttonB) buttonPressedb = false;

    if(toggleEnabledb){
      deploy ();
    }*/

    //remote.ButtonB.released(deploy);



    if (remote.ButtonR1.pressing()) {
     lift.spin(forward);
     liftstopped = false;
    } else if (remote.ButtonR2.pressing()) {
     lift.spin(reverse);
     liftstopped = false;
    } else if (!liftstopped) {
     lift.stop();
     lift.setBrake(hold);
     liftstopped = true;
    }

    if (remote.ButtonUp.pressing()) {
      /*if (!up) {
        goalarm.spinFor(forward, 440, degrees);
        goalarm.setBrake(hold);
        up = true;
      }*/
     clamp.spin(forward);
     clampstopped = false;
    } else if (remote.ButtonDown.pressing()) {
      /*if (up) {
        goalarm.spinFor(reverse, 440, degrees);
        goalarm.setBrake(hold);
        up = false;
      }*/
     clamp.spin(reverse);
     clampstopped = false;
    } else if (!clampstopped) {
     clamp.stop();
     clamp.setBrake(hold);
     clampstopped = true;
    }
    wait(20, msec); // Sleep the task for a short amount of time to
                    // prevent wasted resources.
  }
}

//
// Main will set up the competition functions and callbacks.
//
int main() {
  // Set up callbacks for autonomous and driver control periods.
  Competition.autonomous(autonomous);
  Competition.drivercontrol(usercontrol);

  // Run the pre-autonomous function.
  pre_auton();

  // Prevent main from exiting with an infinite loop.
  while (true) {
    wait(100, msec);
  }
}

