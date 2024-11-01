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
extern motor FishyMech;
extern digital_out Clamp;
extern digital_out Doinker;
extern limit RedirectLimitSwitch;

extern vex::controller::button IntakeButton;
extern vex::controller::button OuttakeButton;
extern vex::controller::button IntakeToRedirectButton;
extern vex::controller::button FishyLiftButton;
extern vex::controller::button FishyLowerButton;
extern vex::controller::button FishyResetButton;
extern vex::controller::button DoinkerButton;
extern vex::controller::button ClampButton;

extern motor_group Left;
extern motor_group Right;

void vexcodeInit(void);