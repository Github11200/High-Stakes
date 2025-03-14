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
motor BackRight = motor(PORT8, ratio6_1, true);
motor BackLeft = motor(PORT16, ratio6_1, false);
motor Intake = motor(PORT12, ratio6_1, true);
motor LadyBrownMotor = motor(PORT15, ratio36_1, true);
digital_out RightDoinker = digital_out(Brain.ThreeWirePort.A);
digital_out LeftDoinker = digital_out(Brain.ThreeWirePort.B);
digital_out Clamp = digital_out(Brain.ThreeWirePort.C);
digital_out DoinkerClamp = digital_out(Brain.ThreeWirePort.D);
optical OpticalSensor = optical(PORT1);
rotation LadyBrownRotation = rotation(PORT10);

vex::controller::button IntakeButton = Controller.ButtonL1;
vex::controller::button LadyBrownRaiseButton = Controller.ButtonL2;
vex::controller::button LadyBrownLoadButton = Controller.ButtonR1;
vex::controller::button LadyBrownLowerButton = Controller.ButtonR2;

vex::controller::button ClampButton = Controller.ButtonDown;
vex::controller::button LeftDoinkerButton = Controller.ButtonRight;
vex::controller::button RightDoinkerButton = Controller.ButtonY;
vex::controller::button OuttakeButton = Controller.ButtonB;

vex::controller::button ClampResetButton = Controller.ButtonX;
vex::controller::button DoinkerClampButton = Controller.ButtonUp;
vex::controller::button RedAllianceButton = Controller.ButtonA;
vex::controller::button BlueAllianceButton = Controller.ButtonLeft;

std::string alliance;

void vexcodeInit(void)
{
  // nothing to initialize
}