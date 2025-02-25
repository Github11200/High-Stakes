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
extern motor Hooks;
extern digital_out Clamp;
extern digital_out Doinker;
extern digital_out FrogMech;
extern optical OpticalSensor;

extern vex::controller::button IntakeButton;
extern vex::controller::button OuttakeButton;
extern vex::controller::button IntakeToFrogButton;
extern vex::controller::button FrogButton;
extern vex::controller::button DoinkerButton;
extern vex::controller::button ClampButton;
extern vex::controller::button ClampResetButton;
extern vex::controller::button DescoreButton;
extern vex::controller::button RedAllianceButton;
extern vex::controller::button BlueAllianceButton;

extern motor_group Left;
extern motor_group Right;

extern std::string alliance;

void vexcodeInit(void);