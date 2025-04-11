using namespace vex;

extern brain Brain;

// VEXcode devices
extern controller Controller;

extern motor FrontRight;
extern motor FrontLeft;
extern motor MiddleRight;
extern motor MiddleLeft;
extern motor BackRight;
extern motor BackLeft;
extern motor Intake;
extern motor LadyBrownMotor;
extern digital_out Clamp;
extern digital_out DoinkerClamp;
extern digital_out RightDoinker;
extern digital_out LeftDoinker;
extern optical OpticalSensor;
extern rotation LadyBrownRotation;

extern vex::controller::button IntakeButton;
extern vex::controller::button OuttakeButton;

extern vex::controller::button LadyBrownRaiseButton;
extern vex::controller::button LadyBrownLoadButton;
extern vex::controller::button LadyBrownForwardButton;
extern vex::controller::button LadyBrownLowerButton;

extern vex::controller::button ClampButton;
extern vex::controller::button ClampResetButton;

extern vex::controller::button LeftDoinkerButton;
extern vex::controller::button RightDoinkerButton;

extern vex::controller::button RedAllianceButton;
extern vex::controller::button BlueAllianceButton;

extern std::string alliance;

void vexcodeInit(void);