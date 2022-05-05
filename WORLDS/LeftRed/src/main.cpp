#include "vex.h"
#include "math.h"
#include <cmath>
#include <algorithm>



double pi = 3.14159;

using namespace vex;

// A global instance of competition
competition Competition;

// define your global instances of motors and other devices here

void setall (int speed, vex::percentUnits units) {
  frontright.setVelocity(speed, units);
  frontleft.setVelocity(speed, units);
  backleft.setVelocity(speed, units);
  backright.setVelocity(speed, units);
  boostleft.setVelocity(speed, units);
  boostright.setVelocity(speed, units);
}

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

void kill () {
  backleft.stop(hold);
  frontleft.stop(hold);
  backright.stop(hold);
  frontright.stop(hold);
  boostleft.stop(hold);
  boostright.stop(hold);
}

void setcoast () {
  backleft.setBrake(vex::coast);
  frontleft.setBrake(vex::coast);
  backright.setBrake(vex::coast);
  frontright.setBrake(vex::coast);
  boostleft.setBrake(vex::coast);
  boostright.setBrake(vex::coast);
}

void spinall () {
  backleft.spin(forward);
  frontleft.spin(forward);
  backright.spin(forward);
  frontright.spin(forward);
  boostleft.spin(forward);
  boostright.spin(forward);
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
  //intertia.calibrate();
  righttracker.setPosition(0, degrees);
  lefttracker.setPosition(0, degrees);
  backleft.resetPosition();
  backright.resetPosition();
  frontright.resetPosition();
  frontleft.resetPosition();
  boostleft.resetPosition();
  boostright.resetPosition();
  arm.resetPosition();
  arm.setBrake(hold);
  kill();
}

//Variables for autonomous
double trackingWheelRadius = 2.75 / 2;
double trackingDistanceLeft = 3;
double trackingDistanceRight = 3;

struct robotpositions {
  double robotx;
  double roboty;
  double robotdistance;
  double robotangle;
};
robotpositions robotposition;

//Tasks for autonomous

double radian (double number) {
  return (number / 180) * pi;
}

double degree (double number) {
  return (number * 180) / pi;
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
  double x = hypotenuse * ( cos( (pi - headingangle) / 2) );
  double y = hypotenuse * ( sin( (pi - headingangle) / 2) );
  double distancexy = sqrt((x*x)+(y*y));
  
  robotposition.robotx = x;
  robotposition.roboty = y;
  robotposition.robotdistance = distancexy;
  robotposition.robotangle = headingangle;

  return robotposition;
}

int printinfo () {
  int rownumber = 1;
  int colnumber = 1;
  while (true) { 
    Brain.Screen.clearScreen();
    getlocation();

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
    Brain.Screen.print("Heading pi radians: %f", (robotposition.robotangle / pi), "pi");

    Brain.Screen.setCursor(rownumber, colnumber);
    rownumber ++;
    Brain.Screen.print("Heading degrees: %f", degree(robotposition.robotangle));

    Brain.Screen.setCursor(rownumber, colnumber);
    rownumber ++;
    Brain.Screen.print("Distance: %f", ultrasonic.distance(inches));

    Brain.Screen.setCursor(rownumber, colnumber);
    rownumber ++;
    Brain.Screen.print("Inertial: %f", inertia.rotation());

    wait(20, msec);
    rownumber = 1;
    colnumber = 1;   
  }
  return 0;
}

void turnto (int target) {
  //turnto() is a working PID that turns the robot the shortest distance given ANY degrees. 
  //It aviods multiple revolutions and will turn in ABSOLUTE coordinates.
  double kp = .25; //tune
  double ki = .055; //tune
  double threshold = 2; //tune
  double accelerator = 18; //tune

  double previousspeed = 0;
  double previouserror = 0;
  double totalerror = 0;
  double error;
  int speed;

  while (true) {
    error = (target - lround(fmod(degree((0 + radian(inertia.rotation())) / 1), 360)) + 540) % 360 - 180;

    if (fabs(error) < threshold) {
      kill();
      break;
      return;
    }

    totalerror = error + previouserror;

    speed = error * kp + totalerror * ki;

    if (speed - previousspeed > accelerator) {
      speed = previousspeed + accelerator;
    }

    setallleft(speed, percent);
    setallright(-speed, percent);

    spinall();

    speed = previousspeed;
    previouserror = error;

    wait(20, msec);    
  }
  kill();
  return;  
}

void moveto (double x, double y) {
  //turnto is a working PID that turns the robot the shortest distance given ANY degrees. 
  //It aviods multiple revolutions and will turn in ABSOLUTE coordinates.
  double kp = .9; //tune
  double ki = .4; //tune
  double threshold = 1; //tune
  double accelerator = 50; //tune
  double previousspeed = 0;
  double previouserror = 0;
  double totalerror = 0;
  double error;
  double errorx;
  double errory;
  int speed;

  double initialx = robotposition.robotx;
  double initialy = robotposition.roboty;

//  double targetdistance = sqrt( pow(x - initialx, 2) + pow(y - initialy, 2));

  while (true) {
    //error = targetdistance - (sqrt( pow(robotposition.robotx - initialx, 2) + pow(robotposition.roboty - initialy, 2)));
    errorx = x - (robotposition.robotx - initialx);
    errory = y - (robotposition.roboty - initialy);

    if (fabs(errorx) < threshold) {
      kill();
      break;
      return;
    }

    if (fabs(errory) < threshold) {
      kill();
      break;
      return;
    }

    error = (errorx + errory)/2;
    totalerror = error + previouserror;

    speed = error * kp + totalerror * ki;

    if (speed - previousspeed > accelerator) {
      speed = previousspeed + accelerator;
    }

    setallleft(speed, percent);
    setallright(speed, percent);

    spinall();

    speed = previousspeed;
    previouserror = error;

    wait(20, msec);    
  }
  kill();
  return;
}

void movetofast(int speed, int target, bool ultorencoder) {
setallleft(speed, percent);
setallright(speed, percent);

if (ultorencoder == false){
while (true) {
spinall();
if (ultrasonic.distance(mm) >= target) {
  break;
  kill();
    }
  }
}
if (ultorencoder == true) {
  double initialtrackerdistance = leftposition();
    while (true) {
    spinall();
     if (leftposition() - initialtrackerdistance >= fabs(target/25.4)) {
    kill();
    break;
  }
}
}

}
double angledestination;

void gotocoord(double x, double y){  
  /*This function just does simple moving to coordinates by rotating first, then traveling, 
  without rotating back to the original orientation.

  This is subject to develop to account for curves, PID, any other fancy thing
  
  Heading angle used here is based off of encoders only, though in loose undeveloped theory, 
  the inertial sensor can be used as well since its angle is just half of the arc angle thing*/

  //double angledestination = ((atan(dispy/dispx))) + robotposition.robotangle; //If inertial sensor is used, omit the '2*' in the beginning.

 double deltax = x - robotposition.robotx;
  double deltay = y - robotposition.roboty;

 // double angledestination = ((atan2(deltax, deltay)) - fmod(robotposition.robotangle, 360));

 if (deltay < 0) {
   angledestination = ((atan2(deltay, deltax)));
 } else {
   angledestination = ((atan2(deltax, deltay)));
 }

  turnto(degree((angledestination)));
  //movetofast(25.4*sqrt((deltax*deltax) + (deltay*deltay)));

  

  //double traveldistance = sqrt((deltax*deltax) + (deltay*deltay));

  //movetofast(traveldistance);
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

  clamp.set(true);
  tilter.set(true);

movetofast(40, 1220, 0);
clamp.set(false);
turnto(180);
movetofast(40, 890 , 1);
turnto()

/*clamp.set(true);
tilter.set(true);
 // gotocoord(24, 24);
  gotocoord(24, 60);
  clamp.set(false);
  turnto(-6);
  setall(-90, percent);
  while (true){
    spinall();
    if (inertia.acceleration(yaxis) < -1){
      break;
    }
    kill();
    tilter.set(false);
  }
*/
 /*turnto(45);
 wait(1, seconds);
 turnto(135);
 wait(1, seconds);
 turnto(180);
 wait(1, seconds);
 turnto(350);
 
*/

 /*gotocoord(48, 48);
 turnto(0);*/
 setcoast();


 

// gotocoord(1,1);


 // gotocoord(24.0, 24.0);
  //wait(1, sec);
  //gotocoord(48, 0);
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
double switchfactor = 1;
double reductionfactor = 1;

int shared = 0;
bool sharedmoving = false;

bool armmoving = false;

bool clamppressed = false;

bool tilterpressed = false;

bool chainpressed = false;
bool chainmoving = false;


void usercontrol(void) {
  arm.setVelocity(0, percent);
  task mytask = task(printinfo);
  kill();
  arm.setBrake(hold);
  while (1) {
    getlocation();

    front = Controller1.Axis3.position(percent);
    rotate = Controller1.Axis4.position(percent);

    if (front < threshold && front > -threshold) {
      front = 0;
    }
    if (rotate < threshold && rotate > -threshold) {
      rotate = 0;
    }

    Brain.Screen.setCursor(10, 1);
    Brain.Screen.print("Axis 3 (forwards-back): %d", front);
    Brain.Screen.setCursor(11, 1);
    Brain.Screen.print("Axis 1 (turning): %d", rotate);

    if (Controller1.ButtonR1.pressing()) {
      shared = -49;
      sharedmoving = true;
    } else if (Controller1.ButtonR2.pressing()) {
      shared = 49;
      sharedmoving = true;
    } else if (sharedmoving) {
      shared = 0;
      sharedmoving = false;
    }

    if (Controller1.ButtonDown.pressing()) {
      switchfactor = .4;
    } else if (Controller1.ButtonUp.pressing()) {
      switchfactor = 1;
    }

    //reductionfactor = std::max(std::max(abs(front - rotate + shared), abs(front + rotate + shared)), std::max(abs(front + rotate - shared), (front - rotate - shared))) / 100;

    
    
    frontright.setVelocity((front - rotate + shared) * switchfactor * reductionfactor, percent);
    frontleft.setVelocity((front + rotate + shared) * switchfactor * reductionfactor, percent);
    backleft.setVelocity((front + rotate - shared) * switchfactor * reductionfactor, percent);
    backright.setVelocity((front - rotate - shared) * switchfactor * reductionfactor, percent);
    boostleft.setVelocity((front + rotate) * switchfactor * reductionfactor, percent);
    boostright.setVelocity((front - rotate) * switchfactor * reductionfactor, percent);
    
    spinall();

    if (Controller1.ButtonL1.pressing()) {
      arm.setVelocity(-100, percent);
      armmoving = true;
    } else if (Controller1.ButtonL2.pressing()) {
      arm.setVelocity(100, percent);
      armmoving = true;
    } else if (armmoving) {
      arm.setVelocity(0, percent);
      arm.setBrake(hold);
      armmoving = false;
    }
    
    arm.spin(forward);
    
    if (Controller1.ButtonX.pressing() && !clamppressed) {
      clamp.set(!clamp.value());
      clamppressed = true;
    } else if (!Controller1.ButtonX.pressing()) {
      clamppressed = false;
    }

    if (Controller1.ButtonA.pressing() && !tilterpressed) {
      tilter.set(!tilter.value());
      tilterpressed = true;
    } else if (!Controller1.ButtonA.pressing()) {
      tilterpressed = false;
    }

    if (Controller1.ButtonB.pressing() && !chainpressed) {
      chain.setVelocity(350, rpm);
      chain.spin(forward);
      chainpressed = true;
      if (chainmoving == false) {
        chain.setVelocity(100, percent);
        chain.spin(forward);
        chainmoving = true;
      } else if (chainmoving == true) {
        chain.setVelocity(0, percent);
        chainmoving = false;
      }
    } else if (!Controller1.ButtonB.pressing()) {
      chainpressed = false;
    }

    

    wait(20, msec);                  
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
