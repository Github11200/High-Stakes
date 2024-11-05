#include "vex.h"

using namespace vex;
using signature = vision::signature;
using code = vision::code;

// A global instance of brain used for printing to the V5 Brain screen
brain Brain;

// VEXcode device constructors
controller Controller;

motor FrontRight = motor(PORT9, ratio6_1, false);
motor FrontLeft = motor(PORT16, ratio6_1, true);
motor MiddleRight = motor(PORT19, ratio6_1, false);
motor MiddleLeft = motor(PORT13, ratio6_1, true);
motor BackRight = motor(PORT14, ratio6_1, false);
motor BackLeft = motor(PORT15, ratio6_1, true);
motor Intake = motor(PORT5, ratio6_1, false);
motor FishyMech = motor(PORT20, ratio36_1, true);
digital_out Clamp = digital_out(Brain.ThreeWirePort.A);
triport ThreeWirePort = vex::triport(vex::PORT22);
digital_out Doinker = digital_out(ThreeWirePort.D);
limit RedirectLimitSwitch = limit(Brain.ThreeWirePort.E);
optical OpticalSensor = optical(PORT18);

vex::controller::button IntakeButton = Controller.ButtonL1;
vex::controller::button OuttakeButton = Controller.ButtonR1;
vex::controller::button FishyLiftButton = Controller.ButtonL2;
vex::controller::button FishyLowerButton = Controller.ButtonY;
vex::controller::button FishyResetButton = Controller.ButtonDown;
vex::controller::button DoinkerButton = Controller.ButtonX;
vex::controller::button ClampButton = Controller.ButtonR2;

motor_group Left = motor_group(FrontLeft, MiddleLeft, BackLeft);
motor_group Right = motor_group(FrontRight, MiddleRight, BackRight);

void vexcodeInit(void)
{
  // nothing to initialize
}