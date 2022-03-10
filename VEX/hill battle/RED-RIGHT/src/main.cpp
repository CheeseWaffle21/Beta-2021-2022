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
  inertia.calibrate();
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
double fwdconversion = 28.6478898;
int fwdaverage () {
  total = ((leftback.position(degrees) + rightback.position(degrees) + rightfront.position(degrees) + leftfront.position(degrees)) / 4);
  Brain.Screen.clearScreen();
  Brain.Screen.setCursor(1, 1);
  Brain.Screen.print(total);
  return total;
}

void end () {
  leftback.stop();
  rightback.stop();
  rightfront.stop();
  leftfront.stop();
  return;
}

void reset () {
  leftback.resetPosition();
  rightback.resetPosition();
  rightfront.resetPosition();
  leftfront.resetPosition();
  return;
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




void fronty (int speed, bool near, int far) {
  leftback.setVelocity(speed, percent);
  rightback.setVelocity(speed, percent);
  leftfront.setVelocity(speed, percent);
  rightfront.setVelocity(speed, percent);
  leftfront.spin(forward);
  rightfront.spin(forward);
  leftback.spin(forward);
  rightback.spin(forward);
  if (near == false) {
    while (true) {
    Brain.Screen.clearScreen();
    Brain.Screen.setCursor(1, 1); 
    Brain.Screen.print(distancer.distance(mm));
    if (distancer.distance(mm) >= far) {
      break; 
    }
    wait(.05, sec);
    }
  }
  if (near == true) {
    while (true) {
    Brain.Screen.clearScreen();
    Brain.Screen.setCursor(1, 1); 
    Brain.Screen.print(distancer.distance(mm));
    if (distancer.distance(mm) <= far) {
      break; 
    }
    wait(.05, sec);
    }
  }
  leftfront.stop();
  rightfront.stop();
  leftback.stop();
  rightback.stop();
  rightfront.setBrake(hold);
  rightback.setBrake(hold);
  leftfront.setBrake(hold);
  leftback.setBrake(hold); 
  return;
}


void grab (bool up) {
  if (up == false){
    clamp.setVelocity(100, percent);
    clamp.rotateFor(-360, degrees, false);   
  } else {
    clamp.setVelocity(100, percent);
    clamp.rotateFor(360, degrees, false);   
  }
}

void kill () {
  leftback.stop(hold);
  rightback.stop(hold);
  rightfront.stop(hold);
  leftfront.stop(hold);
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

    while (true) {
      Brain.Screen.clearScreen();
      Brain.Screen.setCursor(1,1);
      Brain.Screen.print(sin((inertia.rotation(degrees)*pi/180)));
      if (sin((inertia.rotation(degrees)*pi/180)) > (sin((head)*pi/180) - .01) && sin((inertia.rotation(degrees)*pi/180)) < (sin((head)*pi/180)+.01)) {
        break;
   
      }
      wait(5, msec);
    }
      kill();
    }
    if (way==true) {
      leftback.spin(forward);
      rightback.spin(reverse);
      leftfront.spin(forward);
      rightfront.spin(reverse);

      while (true) {
        Brain.Screen.clearScreen();
        Brain.Screen.setCursor(1,1);
        Brain.Screen.print(sin((inertia.rotation(degrees)*pi/180)));
        if (sin((inertia.rotation(degrees)*pi/180)) > (sin((head)*pi/180) - .01) && sin((inertia.rotation(degrees)*pi/180)) < (sin((head)*pi/180)+.01)) {
          break;
        }
       
        wait(5, msec);
      }
        kill();
    }
  }


void pause () {
  rightfront.setBrake(hold);
  rightback.setBrake(hold);
  leftfront.setBrake(hold);
  leftback.setBrake(hold); 
}

void frontpid (vex::directionType dir, double aim, int threshold, int targetangle = NULL) {
  if (targetangle == NULL) {
    targetangle = inertia.rotation(degrees);
  }
  double turnerror = 0;
  bool stay = true;
  double ik = .1; //tune
  double corrector = 0;
  double kp = .2; //tune
  double ki = .00;//02; //tune
  double kd = .35; //tune
  double lasterror = 0;
  double accelerator = 1; //tune
  double error = 0;
  double integral = 0;
  double derivative = 0;
  double speed = 0;
  double previousspeed = 0;
  double target = aim * fwdconversion;
  double tolerance = threshold * fwdconversion;
  reset();
  while (stay == true){
    error = target - fwdaverage();
    integral = integral + error;

    
    derivative = error - lasterror;

    speed = fabs(error * kp) + fabs(integral * ki) + fabs(derivative * kd);
    if (dir == forward) {
      if ((speed - previousspeed) > accelerator) {
        speed = previousspeed + accelerator;
      } else if ((speed - previousspeed) > -accelerator) {
        speed = previousspeed - accelerator;
      }
    } else if (dir == reverse) {
      if ((speed - previousspeed) > accelerator) {
        speed = previousspeed + accelerator;
      } else if ((speed - previousspeed) > -accelerator) {
        speed = previousspeed - accelerator;
      }
    }

    turnerror = (targetangle - inertia.rotation(degrees)) * ik;
    


    leftback.setVelocity(fabs(speed) + turnerror, percent);
    rightback.setVelocity(fabs(speed) - turnerror, percent);
    leftfront.setVelocity(fabs(speed) + turnerror, percent);
    rightfront.setVelocity(fabs(speed) - turnerror, percent);
    leftfront.spin(dir);
    rightfront.spin(dir);
    leftback.spin(dir);
    rightback.spin(dir);

    if (dir == forward) {
      if (error <= tolerance) {
        stay = false;
        break;

      }
    } else if (dir == reverse) {
      if (error >= tolerance) {
        stay = false;
        break;

      }
    }

    lasterror = error;
    previousspeed = speed;

    wait(20, msec);
  }
  kill();
}


void autonomous(void) {
    // ..........................................................................
  // Insert autonomous user code here.
  // ..........................................................................
  //int backspace = 344;
  //int frontspace = 101;

  frontpid(forward, 150, 1);
  frontpid(reverse, -150, 1);

  /*
  Brain.Screen.print("this is autonomous haha");
  //front(-50, false, 1370);
  leftback.setVelocity(-100, percent);
  rightback.setVelocity(-100, percent);
  leftfront.setVelocity(-100, percent);
  rightfront.setVelocity(-100, percent);
  leftfront.spin(forward);
  rightfront.spin(forward);
  leftback.spin(forward);
  rightback.spin(forward);
    while (true) {
    Brain.Screen.clearScreen();
    Brain.Screen.setCursor(1, 1); 
    Brain.Screen.print(distancer.distance(mm));
    if (distancer.distance(mm) >= 1000) {
      break; 
    }
    wait(.05, sec);
    }
  grab(false);
  lift.spinFor(forward, 360, degrees, false);
    while (true) {
    Brain.Screen.clearScreen();
    Brain.Screen.setCursor(1, 1); 
    Brain.Screen.print(distancer.distance(mm));
    if (distancer.distance(mm) >= 1370) {
      break; 
    }
    wait(.05, sec);
    }  
  leftfront.stop();
  rightfront.stop();
  leftback.stop();
  rightback.stop();
  rightfront.setBrake(hold);
  rightback.setBrake(hold);
  leftfront.setBrake(hold);
  leftback.setBrake(hold); 
  rotate2(true, 30, 80);
  rotate2(false, 30, 80);
  rotate3(true, 3, 7);
  front(true, 35, 100);
  leftback.setVelocity(-20, percent);
  rightback.setVelocity(-20, percent);
  leftfront.setVelocity(-20, percent);
  rightfront.setVelocity(-20, percent);

  rotate2(false, 20, 60);

  leftback.setVelocity(-20, percent);
  rightback.setVelocity(-20, percent);
  leftfront.setVelocity(-20, percent);
  rightfront.setVelocity(-20, percent);
  leftfront.spin(forward);
  rightfront.spin(reverse);
  leftback.spin(forward);
  rightback.spin(reverse);
  
  

  while (true) {
    colour.takeSnapshot(colour__REDGOAL);
    Brain.Screen.clearScreen();
    Brain.Screen.setCursor(1, 1); 
    Brain.Screen.print(colour.objectCount);    
    if (colour.largestObject.centerX > 110 && colour.largestObject.centerX < 130) {
      break;
    }
    wait(.05, sec);
  }
  leftfront.stop();
  rightfront.stop();
  leftback.stop();
  rightback.stop();
  rightfront.setBrake(hold);
  rightback.setBrake(hold);
  leftfront.setBrake(hold);
  leftback.setBrake(hold);
  goalarm.setVelocity(100, percent);
  goalarm.spin(reverse);
  while (true) {
    Brain.Screen.clearScreen();
    Brain.Screen.setCursor(1, 1);
    //either .position or .angle 
    Brain.Screen.print(potG.angle(degrees));
    if (potG.angle(degrees) >= 95) {
      break;
    }
    wait(.05, sec);
  }
  goalarm.stop();
  goalarm.setBrake(hold);


  leftback.setVelocity(90, percent);
  rightback.setVelocity(90, percent);
  leftfront.setVelocity(90, percent);
  rightfront.setVelocity(90, percent);
  leftfront.spin(forward);
  rightfront.spin(forward);
  leftback.spin(forward);
  rightback.spin(forward);
  while (true) {
    if (goallimit.pressing()) {
      break; 
    }
    wait(.05, sec);
  }

  goalarm.spin(forward);
  while (true) {
    Brain.Screen.clearScreen();
    Brain.Screen.setCursor(1, 1);
    Brain.Screen.print(potG.angle(degrees));
    if (potG.angle(degrees) <= 23) {
      break;
    }
    wait(1, sec);
  }
  goalarm.stop();
  leftfront.stop();
  rightfront.stop();
  leftback.stop();
  rightback.stop();
  rightfront.setBrake(brake);
  rightback.setBrake(brake);
  leftfront.setBrake(brake);
  leftback.setBrake(brake);
  goalarm.setBrake(hold);

  rightfront.setBrake(coast);
  rightback.setBrake(coast);
  leftfront.setBrake(coast);
  leftback.setBrake(coast);

  
  chain.setVelocity(100, percent);
  chainencoder.setPosition(0, degrees);
  chain.spin(reverse);
  while (true) {
    Brain.Screen.clearScreen();
    Brain.Screen.setCursor(1, 1);
    Brain.Screen.print(chainencoder.position(degrees));
    if (chainencoder.position(degrees) >= 600) {
      break;
    }
    wait(.05, sec);
  }
  chain.stop();

  //Nathan Reese smells like grease
  
 //rotate3(true, 270, 90);

 front(false, 10, 100);

 chain.setVelocity(100, percent);
  chainencoder.setPosition(0, degrees);
  chain.spin(forward);
  while (true) {
    Brain.Screen.clearScreen();
    Brain.Screen.setCursor(1, 1);
    Brain.Screen.print(chainencoder.position(degrees));
    if (chainencoder.position(degrees) <= -2000) {
      break;
    }
    wait(.05, sec);
  }
  chain.stop();
 
  leftback.spin(reverse);
  rightback.spin(forward);
  leftfront.spin(forward);
  rightfront.spin(reverse);

  wait(1.25, seconds);
  leftfront.stop();
  rightfront.stop();
  leftback.stop();
  rightback.stop();
  rightfront.setBrake(coast);
  rightback.setBrake(coast);
  leftfront.setBrake(coast);
  leftback.setBrake(coast);

  Brain.Screen.print("DONE!!");

  
  return;

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

