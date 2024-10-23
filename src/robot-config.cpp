#include "vex.h"

using namespace vex;
using signature = vision::signature;
using code = vision::code;

// A global instance of brain used for printing to the V5 Brain screen
brain Brain;

// VEXcode device constructors
controller Controller;

motor FrontRight = motor(PORT10, ratio6_1, false);
motor FrontLeft = motor(PORT16, ratio6_1, true);
motor MiddleRight = motor(PORT19, ratio6_1, false);
motor MiddleLeft = motor(PORT13, ratio6_1, true);
motor BackRight = motor(PORT14, ratio6_1, false);
motor BackLeft = motor(PORT15, ratio6_1, true);
motor Intake = motor(PORT5, ratio6_1, false);
motor Redirect = motor(PORT2, ratio36_1, false);
digital_out Clamp = digital_out(Brain.ThreeWirePort.A);
triport ThreeWirePort = vex::triport(vex::PORT22);
digital_out Doinker = digital_out(ThreeWirePort.A);
limit RedirectLimitSwitch = limit(Brain.ThreeWirePort.C);

vex::controller::button IntakeButton = Controller.ButtonL1;
vex::controller::button IntakeToRedirectButton = Controller.ButtonR1;
// vex::controller::button IntakeToRedirectButton = Controller.ButtonY;
vex::controller::button RedirectLiftButton = Controller.ButtonL2;
vex::controller::button RedirectLowerButton = Controller.ButtonR2;
vex::controller::button DoinkerButton = Controller.ButtonA;
vex::controller::button ClampButton = Controller.ButtonB;

motor_group Left = motor_group(FrontLeft, MiddleLeft, BackLeft);
motor_group Right = motor_group(FrontRight, MiddleRight, BackRight);

void vexcodeInit(void)
{
  // nothing to initialize
}