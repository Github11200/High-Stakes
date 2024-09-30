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
extern motor Arm;
digital_out Clamp;
digital_out Doinker;

void vexcodeInit(void);