#include "vex.h"

using namespace vex;
using signature = vision::signature;
using code = vision::code;

// A global instance of brain used for printing to the V5 Brain screen
brain Brain;

// VEXcode device constructors
motor left1 = motor(PORT1, ratio6_1, false);
motor left2 = motor(PORT2, ratio6_1, false);
motor left3 = motor(PORT3, ratio6_1, false);
motor_group LeftDrive = motor_group(left1, left2, left3);

motor right1 = motor(PORT11, ratio6_1, true); 
motor right2 = motor(PORT12, ratio6_1, true); 
motor right3 = motor(PORT13, ratio6_1, true);
motor_group RightDrive = motor_group(right1, right2, right3);

controller Controller1 = controller(primary);

motor Intake = motor(PORT4, ratio6_1, false);
motor Puncher = motor(PORT5, ratio18_1, false);

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