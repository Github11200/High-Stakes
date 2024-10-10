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
extern motor Redirect;
extern digital_out Clamp;
extern digital_out Doinker;
extern limit RedirectLimitSwitch;
extern motor_group Left;
extern motor_group Right;

void vexcodeInit(void);