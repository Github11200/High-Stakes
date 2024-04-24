/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Author:       VEX                                                       */
/*    Created:      Wed Sep 25 2019                                           */
/*    Description:  Clawbot Template (4-motor Drivetrain, No Gyro)            */
/*                                                                            */
/*    Name:                                                                   */
/*    Date:                                                                   */
/*    Class:                                                                  */
/*                                                                            */
/*----------------------------------------------------------------------------*/

// ---- START VEXCODE CONFIGURED DEVICES ----
// Robot Configuration:
// [Name]               [Type]        [Port(s)]
// FrontLeft            motor         1               
// MiddleLeft           motor         2               
// BackLeft             motor         3               
// FrontRight           motor         8               
// MiddleRight          motor         9               
// BackRight            motor         10              
// Puncher              motor         20              
// Intake               motor         11              
// Inertial             inertial      16              
// Controller1          controller                    
// Wings                digital_out   G               
// Balance              digital_out   H               
// PuncherB             motor         13              
// ---- END VEXCODE CONFIGURED DEVICES ----

#include "vex.h"
#include <iostream>

using namespace vex;
using namespace std;
competition Competition;

void pre_auton(void) {
  vexcodeInit();
}

void autonomous(void) {
}

void usercontrol(void) {
  while (1) {
  }
}

int main() {
  Competition.autonomous(autonomous);
  Competition.drivercontrol(usercontrol);

  pre_auton();

  while (true) {
    wait(100, msec);
  }
}
