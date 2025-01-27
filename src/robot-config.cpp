#include "vex.h"

using namespace vex;
using signature = vision::signature;
using code = vision::code;

// A global instance of brain used for printing to the V5 Brain screen
brain Brain;

// VEXcode device constructors
controller Controller;

motor FrontRight = motor(PORT18, ratio6_1, true);
motor FrontLeft = motor(PORT17, ratio6_1, false);
motor MiddleRight = motor(PORT19, ratio6_1, false);
motor MiddleLeft = motor(PORT14, ratio6_1, true);
motor BackRight = motor(PORT8, ratio6_1, false);
motor BackLeft = motor(PORT16, ratio6_1, true);
motor Intake = motor(PORT12, ratio6_1, true);
motor Hooks = motor(PORT3, ratio6_1, false);
digital_out Clamp = digital_out(Brain.ThreeWirePort.B);
digital_out Doinker = digital_out(Brain.ThreeWirePort.A);
digital_out FrogMech = digital_out(Brain.ThreeWirePort.C);
triport ThreeWirePort = vex::triport(vex::PORT22);
optical OpticalSensor = optical(PORT1);

vex::controller::button IntakeButton = Controller.ButtonL1;
vex::controller::button OuttakeButton = Controller.ButtonY;
vex::controller::button FrogButton = Controller.ButtonL2;
vex::controller::button IntakeToFrogButton = Controller.ButtonR1;
vex::controller::button DoinkerButton = Controller.ButtonUp;
vex::controller::button ClampButton = Controller.ButtonR2;
vex::controller::button DescoreButton = Controller.ButtonA;

motor_group Left = motor_group(FrontLeft, MiddleLeft, BackLeft);
motor_group Right = motor_group(FrontRight, MiddleRight, BackRight);

std::string alliance;

void vexcodeInit(void)
{
  // nothing to initialize
}