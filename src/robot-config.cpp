#include "vex.h"

using namespace vex;
using signature = vision::signature;
using code = vision::code;

// A global instance of brain used for printing to the V5 Brain screen
brain Brain;

// VEXcode device constructors
controller Controller;

motor FrontRight = motor(PORT18, ratio6_1, false);
motor FrontLeft = motor(PORT12, ratio6_1, true);
motor MiddleRight = motor(PORT7, ratio6_1, true);
motor MiddleLeft = motor(PORT11, ratio6_1, false);
motor BackRight = motor(PORT17, ratio6_1, false);
motor BackLeft = motor(PORT15, ratio6_1, true);
motor Intake = motor(PORT8, ratio6_1, false);
motor LadyBrownMotor = motor(PORT2, ratio36_1, false);
digital_out RightDoinker = digital_out(Brain.ThreeWirePort.B);
digital_out LeftDoinker = digital_out(Brain.ThreeWirePort.C);
digital_out Clamp = digital_out(Brain.ThreeWirePort.A);
digital_out DoinkerClamp = digital_out(Brain.ThreeWirePort.D);
optical OpticalSensor = optical(PORT5);
rotation LadyBrownRotation = rotation(PORT16, true);

vex::controller::button IntakeButton = Controller.ButtonL1;
vex::controller::button OuttakeButton = Controller.ButtonX;

vex::controller::button LadyBrownRaiseButton = Controller.ButtonL2;
vex::controller::button LadyBrownLoadButton = Controller.ButtonR1;
vex::controller::button LadyBrownForwardButton = Controller.ButtonRight;
vex::controller::button LadyBrownLowerButton = Controller.ButtonY;

vex::controller::button ClampButton = Controller.ButtonR2;
vex::controller::button ClampResetButton = Controller.ButtonUp;

vex::controller::button LeftDoinkerButton = Controller.ButtonDown;
vex::controller::button RightDoinkerButton = Controller.ButtonB;

vex::controller::button RedAllianceButton = Controller.ButtonA;
vex::controller::button BlueAllianceButton = Controller.ButtonLeft;

std::string alliance;

void vexcodeInit(void)
{
  // nothing to initialize
}