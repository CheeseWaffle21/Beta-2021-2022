/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Author:       C:\Users\Lamar                                            */
/*    Created:      Wed Mar 09 2022                                           */
/*    Description:  V5 project                                                */
/*                                                                            */
/*----------------------------------------------------------------------------*/

// ---- START VEXCODE CONFIGURED DEVICES ----
// ---- END VEXCODE CONFIGURED DEVICES ----

#include "vex.h"

using namespace vex;

int total;
int fwdconversion = 28; //28 degrees travels 1 inch
int strafeconversion = 62; //62 degrees strafes 1 inch
double trnconversion = 17.5; //17.5 degrees turns some amount of degrees idk

int main() {
  // Initializing Robot Configuration. DO NOT REMOVE!
  vexcodeInit();
  
}

  double current;
  double kP = 0.2;
  double kI;
  double kD;
  double target;
  double error;
  double integral;
  double derivative;
  double lastError;
  double threshold;
  int lastTime;

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

int fwdaverage () {
  total = ((leftback.position(degrees) + rightback.position(degrees) + rightfront.position(degrees) + leftfront.position(degrees)) / 4);
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

    target = number * fwdconversion;
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

    target = number * -fwdconversion;
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
  wait(.2, seconds);
  return;
}




void autonomous(void){

}
