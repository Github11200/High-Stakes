#include "vex.h"

using namespace vex;
using signature = vision::signature;
using code = vision::code;

// A global instance of brain used for printing to the V5 Brain screen
brain Brain;

// VEXcode device constructors
controller Controller;

motor FrontRight = motor(PORT19, ratio6_1, false);
motor FrontLeft = motor(PORT10, ratio6_1, true);
motor MiddleRight = motor(PORT17, ratio6_1, false);
motor MiddleLeft = motor(PORT9, ratio6_1, true);
motor BackRight = motor(PORT18, ratio6_1, false);
motor BackLeft = motor(PORT8, ratio6_1, true);
motor Intake = motor(PORT1, ratio6_1, false);
motor Redirect = motor(PORT2, ratio36_1, false);
digital_out Clamp = digital_out(Brain.ThreeWirePort.A);
digital_out Doinker = digital_out(Brain.ThreeWirePort.B);

void vexcodeInit(void)
{
    // nothing to initialize
}