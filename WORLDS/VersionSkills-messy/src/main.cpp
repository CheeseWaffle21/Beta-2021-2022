// ---- START VEXCODE CONFIGURED DEVICES ----
// Robot Configuration:
// [Name]               [Type]        [Port(s)]
// frontleft            motor         7               
// backleft             motor         6               
// backright            motor         10              
// frontright           motor         8               
// Controller1          controller                    
// boostright           motor         14              
// boostleft            motor         13              
// clamp                digital_out   C               
// inertia              inertial      17              
// righttracker         encoder       A, B            
// lefttracker          encoder       G, H            
// arm                  motor         16              
// expander             triport       11              
// tilter               digital_out   D               
// ultrasonic           sonar         A, B            
// chain                motor         18              
// LimitSwitchC         limit         C               
// ---- END VEXCODE CONFIGURED DEVICES ----
// ---- START VEXCODE CONFIGURED DEVICES ----
// Robot Configuration:
// [Name]               [Type]        [Port(s)]
// frontleft            motor         7               
// backleft             motor         6               
// backright            motor         10              
// frontright           motor         8               
// Controller1          controller                    
// boostright           motor         14              
// boostleft            motor         13              
// clamp                digital_out   C               
// inertia              inertial      17              
// righttracker         encoder       A, B            
// lefttracker          encoder       G, H            
// arm                  motor         16              
// expander             triport       11              
// tilter               digital_out   D               
// ultrasonic           sonar         A, B            
// chain                motor         18              
// LimitSwitchF         limit         F               
// ---- END VEXCODE CONFIGURED DEVICES ----
#include "vex.h"
#include "math.h"
#include <cmath>
#include <algorithm>
#include <iostream>;


double pi = 3.14159;




using namespace vex;

// A global instance of competition
competition Competition;

// define your global instances of motors and other devices here

int shared = 0;
bool sharedmoving = false;

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
  double hypotenuse = sqrt( pow(radius, 2) + pow(radius, 2) - ( 2 * radius * radius * cos(inertia.rotation()) ) );
  
  //double x = hypotenuse * ( cos( (pi - headingangle) / 2) );
  //double y = hypotenuse * ( sin( (pi - headingangle) / 2) );
  
  double x = hypotenuse * sin(headingangle);
  double y = hypotenuse * cos(headingangle);

  robotposition.robotx = x;
  robotposition.roboty = y;
  robotposition.robotdistance = hypotenuse;
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
    Brain.Screen.print("Distance: %f", ultrasonic.distance(mm));

    Brain.Screen.setCursor(rownumber, colnumber);
    rownumber ++;
    Brain.Screen.print("Inertial: %f", inertia.rotation(degrees));

      printf("test currentx %f\n", robotposition.robotx);

    wait(20, msec);
    rownumber = 1;
    colnumber = 1;   
  }
  return 0;
}

void turnto (int target, int speed) {
  //turnto() is a working PID that turns the robot the shortest distance given ANY degrees. 
  //It aviods multiple revolutions and will turn in ABSOLUTE coordinates.
  double kp = .4; //tune
  double ki = .06; //tune
  double threshold = 2; //tune
  double accelerator = 18; //tune

  double previousspeed = 0;
  double previouserror = 0;
  double totalerror = 0;
  double error;
 


 //error = (degree(target) - lround(fmod(degree((0 + radian(inertia.rotation())) / 1), 360)) + 540) % 360 - 180;
 error = (target - (fmod(inertia.rotation(degrees), 360)));
 setallleft(speed, percent);
  setallright(-speed, percent);

  while (true) {
    //error = (target - lround(fmod(degree((0 + radian(inertia.rotation())) / 1), 360)) + 540) % 360 - 180;
  
 
     spinall();

  if ((fmod(inertia.rotation(degrees), 360)) >= error) {
    kill();
    break;
    return;
  }
    
/*
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

  //  setallleft(speed, percent);
  //  setallright(-speed, percent);

    spinall();

    previousspeed = speed;
    previouserror = error;

    wait(20, msec);    
  }
  kill();
  return; */ 
  }
}

void moveto (double dist) {
  //turnto is a working PID that turns the robot the shortest distance given ANY degrees. 
  //It aviods multiple revolutions and will turn in ABSOLUTE coordinates.
  double kp = 1; //tune
  double ki = 0; //tune
  double threshold = 1; //tune
  double accelerator = 50; //tune
  double previousspeed = 0;
  double previouserror = 0;
  double totalerror = 0;
  double error;
  int speed;

  double initialdist = robotposition.robotdistance;
 

//  double targetdistance = sqrt( pow(x - initialx, 2) + pow(y - initialy, 2));

  while (true) {
    //error = targetdistance - (sqrt( pow(robotposition.robotx - initialx, 2) + pow(robotposition.roboty - initialy, 2)));
    error = dist - (robotposition.robotdistance - initialdist);
   

    if (fabs(error) < threshold) {
      kill();
      break;
      return;
    }


    totalerror = error + previouserror;

    speed = error * kp + totalerror * ki;

    /*if (speed - previousspeed > accelerator) {
      speed = previousspeed + accelerator;
    }*/

    setallleft(1.5*speed, percent);
    setallright(1.5*speed, percent);

    spinall();

    previousspeed = speed;
    previouserror = error;

    wait(20, msec);    
  }
  kill();
  return;
}

double angledestination;

double totalx = 0;
double totaly = 0;
double lastx = 0;
double lasty = 0;
double deltaheading = 0;
double dtheta = 0;
double deltahyp = 0;

void gotocoord(double x, double y){  
  /*This function just does simple moving to coordinates by rotating first, then traveling, 
  without rotating back to the original orientation.

  This is subject to develop to account for curves, PID, any other fancy thing
  
  Heading angle used here is based off of encoders only, though in loose undeveloped theory, 
  the inertial sensor can be used as well since its angle is just half of the arc angle thing*/

  //double angledestination = ((atan(dispy/dispx))) + robotposition.robotangle; //If inertial sensor is used, omit the '2*' in the beginning.

  double initialrobotdistance = robotposition.robotdistance;
  double initialrobotangle = radian(inertia.rotation(degrees));

 // currentx = deltahyp * sin(robotposition.robotangle) + lastx;
 // currenty = deltahyp * cos(robotposition.robotangle) + lasty;

 double deltax = x - lastx; //change in target x and last x
 double deltay = y - lasty;

 // double angledestination = ((atan2(deltax, deltay)) - fmod(robotposition.robotangle, 360));


   angledestination = ((atan2(deltax, deltay)));
 

//turnhere


  turnto(degree((angledestination)), 20);
 /* lastx = currentx;
  lasty = currenty; 
  
  Les said to put lastx and y here for better accuracy*/

  //printf("test currentx %f\n", totalx);
  moveto(sqrt((deltax*deltax) + (deltay*deltay)));


  deltahyp = robotposition.robotdistance - initialrobotdistance;
  double deltaxreal = deltahyp*sin(radian(inertia.rotation(degrees))); //actual change in x after rotation and translation
  double deltayreal = deltahyp*cos(radian(inertia.rotation(degrees)));

  totalx = deltaxreal + lastx;
  totaly = deltayreal + lasty;

  //deltahyp = robotposition.robotdistance - initialhyp;
  dtheta = robotposition.robotangle - initialrobotangle;
  deltahyp = robotposition.robotdistance - initialrobotdistance;

  lastx = totalx;
  lasty = totaly;

  
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
  ///*
  task mytask = task(printinfo);
  gotocoord(15,15);
  wait(2, sec);
  gotocoord(30,15);

 /*clamp.set(true);
 tilter.set(true);

  task mytask = task(printinfo);
moveto(-24);
tilter.set(false);
chain.setVelocity(100, percent);
chain.spin(forward);
moveto(48);
turnto(90);*/

/*moveto(56);
clamp.set(false);
turnto(120);
moveto(30);
shared = -49;
sharedmoving = true;
*/

  
  
 /*setall(100, percent);
  spinall();
  clamp.set(true);
  
  while (true) {
    if (LimitSwitchC.pressing() == true || ultrasonic.distance(mm) >= 1280) {
      kill();
      clamp.set(false);
      break;
    }
   wait (20, msec);
  }

  turnto(180);

  setall(80, percent);

  double initialleft = leftposition();

  spinall();

  while (true) {
    if (leftposition() - initialleft >= 29) {
      setcoast();
      break;
      return;
    }
    wait(20, msec);
  }

  tilter.set(true);
  turnto(-90);
  
  setall(-80, percent);
  spinall();
  
  initialleft = leftposition();

  spinall();

  while (true) {
    if (initialleft - leftposition() >= 2) {
      setcoast();
      tilter.set(false);
      break;
      return;
    }
    wait(20, msec);
  }
  //
  arm.spinFor(forward, 300, degrees);

  setall(100, percent);

  backleft.spinFor(forward, 400, degrees, false);
  backright.spinFor(forward, 400, degrees, false);
  frontleft.spinFor(reverse, 400, degrees, false);
  frontright.spinFor(reverse, 400, degrees, false);
  ///*
  chain.setVelocity(100, percent);
  chain.spin(forward);
  
  turnto(0);  

  initialleft = leftposition();
  setall(80, percent);
  spinall();

  while (true) {
    if (leftposition() - initialleft >= 12) {
      setcoast();
      break;
      return;
    }
    wait(20, msec);
    
  }  
  //*/
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
      Controller1.Screen.print("Clamp Closed");
    } else if (!Controller1.ButtonX.pressing()) {
      clamppressed = false;
      Controller1.Screen.print("Clamp Open");
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
