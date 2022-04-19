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
// frontleft            motor         8               
// backleft             motor         11              
// backright            motor         9               
// frontright           motor         10              
// Controller1          controller                    
// boostright           motor         20              
// boostleft            motor         12              
// clamp                digital_out   F               
// intertia             inertial      1               
// righttracker         encoder       A, B            
// lefttracker          encoder       G, H            
// ---- END VEXCODE CONFIGURED DEVICES ----

#include "vex.h"
#include <math.h>
#include <cmath>


double pi = 3.14159;

using namespace vex;

// A global instance of competition
competition Competition;

// define your global instances of motors and other devices here

void setallright (int speed, vex::percentUnits units) {
  backright.setVelocity(speed, units);
  frontright.setVelocity(speed, units);
  boostright.setVelocity(speed, units);
}
void setallleft (int speed, vex::percentUnits units) {
  backleft.setVelocity(speed, units);
  frontleft.setVelocity(speed, units);
  boostleft.setVelocity(speed, units);
}
void spinall () {
  backleft.spin(forward);
  frontleft.spin(forward);
  backright.spin(forward);
  frontright.spin(forward);
  boostleft.spin(forward);
  boostright.spin(forward);
}

void clampy() {

}

void spinreverse () {

  backleft.spin(reverse);
  frontleft.spin(reverse);
  backright.spin(reverse);
  boostleft

}
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

  //Sensor actions
  righttracker.setPosition(0, degrees);
  lefttracker.setPosition(0, degrees);
  //intertia.calibrate();
}

//Variables for autonomous
double trackingWheelRadius = 2.75 / 2;
double trackingDistanceLeft = 3;
double trackingDistanceRight = 3;
struct robotpositions {
  double robotx;
  double roboty;
  double robotangle;
};
robotpositions robotposition;

//Tasks for autonomous

double radian (double number) {
  return (number / 180) * pi;
}

double leftposition () {
  return radian(lefttracker.position(degrees)) * trackingWheelRadius;
}

double rightposition () {
  return radian(righttracker.position(degrees)) * trackingWheelRadius;
}

double getheading () {
  return ( leftposition() - rightposition() ) / ( trackingDistanceRight + trackingDistanceLeft );
}

robotpositions getlocation () {
  double headingangle = getheading();
  double rightvalue = rightposition();
  double leftvalue = leftposition();
  double radius = ( ( rightvalue * trackingDistanceLeft ) + ( leftvalue * trackingDistanceRight ) ) / ( leftvalue - rightvalue );
  double hypotenuse = sqrt( pow(radius, 2) + pow(radius, 2) - ( 2 * radius * radius * cos(headingangle) ) );
  double x = hypotenuse * ( sin(90 - ( (180 - headingangle) / 2 ) ) );
  double y = hypotenuse * ( sin( (180 - headingangle) / 2) );
  
  robotposition.robotx = x;
  robotposition.roboty = y;
  robotposition.robotangle = headingangle;

  return robotposition;
}

int printinfo () {
  int rownumber = 1;
  int colnumber = 1;
  while (true) { 
    Brain.Screen.clearScreen();

    Brain.Screen.setCursor(rownumber, colnumber);
    rownumber ++;
    Brain.Screen.print("Left tracker value: %f", leftposition());

    Brain.Screen.setCursor(rownumber, colnumber);
    rownumber ++;
    Brain.Screen.print("Right tracker value: %f", rightposition());

    Brain.Screen.setCursor(rownumber, colnumber);
    rownumber ++;
    Brain.Screen.print("X: %f", robotposition.robotx);
    
    Brain.Screen.setCursor(rownumber, colnumber);
    rownumber ++;
    Brain.Screen.print("Y: %f", robotposition.roboty);

    Brain.Screen.setCursor(rownumber, colnumber);
    rownumber ++;
    Brain.Screen.print("Heading: %f", robotposition.robotangle);

    wait(20, msec);
    rownumber = 1;
    colnumber = 1;   
  }
  return 0;
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

void autonomous(void) {
  // ..........................................................................
  // Insert autonomous user code here.
  // ..........................................................................
  task mytask = task(printinfo);
  while (true) {
    getlocation();
    wait(20, msec);
  }

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
int threshold = 10; //tune
int front = 0;
int rotate = 0;
int leftspeed = 0;
int rightspeed = 0;

int shared = 0;
bool sharedmoving = false;


void usercontrol(void) {
  Brain.Screen.print("hello");
  // User control code here, inside the loop
  while (1) {
    // This is the main execution loop for the user control program.
    // Each time through the loop your program should update motor + servo
    // values based on feedback from the joysticks.

    // ........................................................................
    // Insert user code here. This is where you use the joystick values to
    // update your motors, etc.
    // ........................................................................
    //Brain.Screen.print("hello");

    front = Controller1.Axis3.position(percent);
    rotate = Controller1.Axis4.position(percent);

    if (front < threshold && front > -threshold) {
      front = 0;
    }
    if (rotate < threshold && rotate > -threshold) {
      rotate = 0;
    }

    Brain.Screen.clearScreen();
    Brain.Screen.setCursor(1, 1);
    Brain.Screen.print("Axis 3: %d", front);
    Brain.Screen.setCursor(2, 1);
    Brain.Screen.print("Axis 1: %d", rotate);

    if (Controller1.ButtonL1.pressing()) {
      shared = 100;
      sharedmoving = true;
    } else if (Controller1.ButtonL2.pressing()) {
      shared = -100;
      sharedmoving = true;
    } else if (sharedmoving) {
      shared = 0;
    }

    
    if (Controller1.ButtonX.pressing()) {
      
      clamp.set(true);

    } else if (Controller1.ButtonB.pressing()) {

      clamp.set(false);

    }

    leftspeed = front + rotate;
    rightspeed = front - rotate;



    setallleft(front + rotate, percent);
    setallright(front - rotate, percent);


    spinall();

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
