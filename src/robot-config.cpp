#include "vex.h"

using namespace vex;
using signature = vision::signature;
using code = vision::code;

// A global instance of brain used for printing to the V5 Brain screen
brain Brain;

// VEXcode device constructors
controller Controller;

motor FrontRight = motor(PORT19, ratio6_1, false);  // Done
motor FrontLeft = motor(PORT10, ratio6_1, true);    // Done
motor MiddleRight = motor(PORT17, ratio6_1, false); // Done
motor MiddleLeft = motor(PORT9, ratio6_1, true);    // Done
motor BackRight = motor(PORT18, ratio6_1, false);   // Done
motor BackLeft = motor(PORT8, ratio6_1, true);      // Done

void vexcodeInit(void)
{
    // nothing to initialize
}