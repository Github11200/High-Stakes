#include "vex.h"

using namespace vex;
using signature = vision::signature;
using code = vision::code;

// A global instance of brain used for printing to the V5 Brain screen
brain Brain;

// VEXcode device constructors
motor FrontLeft = motor(PORT1, ratio6_1, false);
motor MiddleLeft = motor(PORT2, ratio6_1, false);
motor BackLeft = motor(PORT3, ratio6_1, false);

motor FrontRight = motor(PORT11, ratio6_1, true); 
motor MiddleRight = motor(PORT12, ratio6_1, true); 
motor BackRight = motor(PORT13, ratio6_1, true);

// VEXcode generated functions

bool RemoteControlCodeEnabled = true;

/**
 * Used to initialize code/tasks/devices added using tools in VEXcode Pro.
 * 
 * This should be called at the start of your int main function.
 */
void vexcodeInit( void ) {
  // nothing to initialize
}