#include "vex.h"
#include "../include/driver/intake.h"
#include "../include/driver/ladyBrown.h"
#include "../include/driver/mogo.h"
#include "../include/auto/skills.h"
#include "../include/auto/negativeRingRush.h"
#include "../include/auto/positiveGoalRush.h"

using namespace vex;
using namespace std;

bool intakeToLadyBrownAuton = false;
bool intakeSort = false;
bool intakeRev = false;
bool ladyBrownScore = false;
bool ladyBrownAllianceStakeScore = false;
bool ladyBrownAllianceHoldUp = false;
float ladyBrownDelay = 0;
bool mogoClamp = false;
float mogoClampDelay = 0;
IntakeControl intakeControl(12);
LadyBrown ladyBrown(5, 166, 212, 116, 6, 0.3, 1.5, 0.5, 10, 1500, 12, 360, 2);
MogoControl mogoControl;

vector<Point> Autonomous::mirrorPath(vector<Point> originalPath)
{
  for (int i = 0; i < originalPath.size(); ++i)
    originalPath[i].x = -originalPath[i].x;
  return originalPath;
}

DriveParams goal_constants()
{
  DriveParams goalParams;
  goalParams.set_max_voltage(12).set_kp(0.7).set_ki(0.01).set_kd(3.5).set_starti(3);
  goalParams.set_heading_max_voltage(10).set_heading_kp(0.15).set_heading_ki(0.02).set_heading_kd(1).set_heading_starti(1);

  return goalParams;
}

ExitConditions fast_exit_conditions()
{
  ExitConditions fastExitConditions;
  fastExitConditions.driveParams.set_settle_error(2).set_settle_time(0.01).set_timeout(4000);
  fastExitConditions.turnParams.set_settle_error(2).set_settle_time(0.01).set_timeout(800);
  fastExitConditions.swingParams.set_settle_error(1).set_settle_time(5000).set_timeout(5000);

  return fastExitConditions;
}

ExitConditions exact_exit_conditions()
{
  ExitConditions exactExitConditions;
  exactExitConditions.driveParams.set_settle_error(0.5).set_settle_time(200).set_timeout(4000);
  exactExitConditions.turnParams.set_settle_error(1).set_settle_time(200).set_timeout(1200);
  exactExitConditions.swingParams.set_settle_error(1).set_settle_time(5000).set_timeout(5000);

  return exactExitConditions;
}

Autonomous::Autonomous()
{
  this->allianceColor = allianceColor;

  static IntakeControl *intakeControlThingy = &intakeControl;
  static LadyBrown *ladyBrownThingy = &ladyBrown;
  static MogoControl *mogoControlThingy = &mogoControl;
  this->intakeAutonTask = thread([]()
                                 { while (true) {
                                    intakeControlThingy->intakeAutonTask();
                                    wait(50, vex::timeUnits::msec);
                                  } });
  // this->ladyBrownAutonTask = thread([]()
  //                                   { while (true) {
  //                                       ladyBrownThingy->ladyBrownAutonTask();
  //                                       wait(50, vex::timeUnits::msec);
  //                                     } });
  this->mogoAutonTask = thread([]()
                               {
                                  while (true) {
                                    mogoControlThingy->mogoAutonTask();
                                    wait(50, vex::timeUnits::msec);
                                  } });

  this->driveParamsWithMogo.set_kp(driveParams.drive_kp + 0.05);
  this->driveParamsWithMogo.set_kd(driveParams.drive_kd + 0.5);
  this->driveParamsWithMogo.set_drive_slew(driveParams.drive_slew - 0.65);

  this->turnParamsWithMogo.set_kp(turnParams.turn_kp - 0.017);
}

Autonomous::~Autonomous()
{
  this->intakeAutonTask.interrupt();
  this->intakeAutonTask.~thread();

  this->ladyBrownAutonTask.interrupt();
  this->ladyBrownAutonTask.~thread();

  cout << "Killed it" << endl;
}

void Autonomous::setAllianceColor(vex::color allianceColor) { this->allianceColor = allianceColor; }

// NOT TESTED
void Autonomous::solo_awp()
{
  int reversed;

  LadyBrownRotation.setPosition(29.4, vex::rotationUnits::deg);

  if (this->allianceColor == vex::color::red)
  {
    reversed = 1;
    alliance = "red";
    chassis.set_coordinates(-57.00, 7.6944, 225);
  }
  else
  {
    reversed = -1;
    alliance = "blue";
    // FIX BLUE COORDINATES
    chassis.set_coordinates(56.548, 11.855, 135);
  }

  cout << "solo awp auto started, initial position:" << endl;
  cout << chassis.get_X_position() << ", " << chassis.get_Y_position() << endl;

  // Score on alliance stake
  ladyBrown.allianceStakeScore();

  // Drive in front of the mogo
  chassis.drive_to_point(-38.3886, 27.6128, DriveParams().set_timeout(1000));

  // Pull the lady brown back
  static LadyBrown ladyBrownPointerThing = ladyBrown;
  thread autonLoadingThread = thread([]()
                                     { ladyBrown.autonLoading(); cout << "brought it back..." << endl; this_thread::yield(); });

  // Turn towards the mogo
  chassis.turn_to_point(-20.099, 16.6367, 180, this->turnParams.set_timeout(500));

  // Curve into goal and clamp
  mogoClampDelay = 1000;
  mogoClamp = true;

  // Drive into the mogo and clamp
  chassis.drive_to_point(-20.099, 16.6367, DriveParams().set_min_voltage(2).set_timeout(600).set_settle_time(0).set_settle_error(0.5));

  intakeSort = true;

  // Go to the rings in the middle
  chassis.turn_to_point(-7.47664, 32.7364, 0, this->turnParams.set_timeout(200));
  LeftDoinker.set(true);
  DriveParams temporaryDriveWithMogoParams = this->driveParamsWithMogo;
  chassis.drive_to_point(-7.47664, 32.7364, temporaryDriveWithMogoParams.set_timeout(800));

  // Pull the ring back with the doinker
  temporaryDriveWithMogoParams = this->driveParamsWithMogo;
  chassis.turn_to_point(-33.1289, 0.491801, 180, this->turnParams.set_timeout(100).set_settle_time(0).set_settle_error(1));
  chassis.drive_to_point(-33.1289, 0.491801, temporaryDriveWithMogoParams.set_timeout(800).set_settle_time(0).set_settle_error(0.5).set_min_voltage(2));

  LeftDoinker.set(false);

  // Score the other two rings which are in a line
  temporaryDriveWithMogoParams = this->driveParamsWithMogo;
  chassis.turn_to_point(-25.1708, 41.9966, 0, this->turnParams.set_timeout(100));
  chassis.drive_to_point(-25.1708, 41.9966, temporaryDriveWithMogoParams.set_timeout(1000).set_settle_time(0).set_settle_error(0.1).set_min_voltage(5));

  // Turn towards the point from which we align to the 2 stack of rings
  chassis.turn_to_point(-49.9043, 15.9103, 0, this->turnParams.set_timeout(100));
  mogoClamp = false;
  Clamp.set(false);
  chassis.drive_to_point(-49.9043, 15.9103, DriveParams().set_timeout(600).set_settle_time(0).set_settle_error(2).set_min_voltage(5));

  // Move into the stack and intake it
  chassis.turn_to_point(-49.6156, -27.4898, 0, this->turnParams.set_timeout(100));
  thread intakeStackThread = thread([]()
                                    {
                                      // We've already intaked the ring we don't want and are now intaking the one we do want
                                      if (!intakeControl.shouldEjectRing()) {
                                        intakeSort = false;
                                        this_thread::yield();
                                      }
                                      wait(20, vex::timeUnits::msec); });
  chassis.drive_to_point(-49.6156, -27.4898, DriveParams().set_timeout(700).set_settle_time(0).set_settle_error(1).set_min_voltage(2));

  // Turn towards the second mogo and drive into it
  chassis.turn_to_point(-23.2287, -28.8152, 180, this->turnParams.set_timeout(400));
  mogoClampDelay = 1200;
  mogoClamp = true;
  chassis.drive_to_point(-23.2287, -28.8152, DriveParams().set_timeout(800).set_settle_time(0).set_settle_error(0.5).set_min_voltage(2));
  intakeSort = true;

  LadyBrownMotor.spin(vex::directionType::fwd, 12, vex::voltageUnits::volt);
  chassis.turn_to_point(-21.2926, -26.553, 0, this->turnParams.set_timeout(200));
}

// NOT TESTED
void Autonomous::positive_six_ring()
{
  int reversed;

  if (this->allianceColor == vex::color::red)
  {
    chassis.set_coordinates(-51.312, -29.859, 78);
    reversed = -1;
    alliance = "red";
  }
  else
  {
    chassis.set_coordinates(51.312, -29.859, 282);
    reversed = 1;
    alliance = "blue";
  }

  cout << "positive six ring started, initial position:" << endl;
  cout << chassis.get_X_position() << ", " << chassis.get_Y_position() << ", " << chassis.get_absolute_heading() << endl;

  // Drive back and clamp the goal
  mogoClampDelay = 800;
  mogoClamp = true;
  chassis.drive_to_point(23.701, -23.701);
  intakeSort = true;

  // Go to the center and doinker two rings
  chassis.turn_to_point(9.995, -10.79, 0);
  chassis.drive_to_point(9.995, -10.79);
  LeftDoinker.set(true);
  chassis.turn_to_angle(300);
  chassis.drive_distance(5, 300);
  RightDoinker.set(true);

  // Go back, lining up the doinkered rings in a smooth path for later
  chassis.drive_to_pose(40.784, -35.421, 330, 0.5, 0);
  LeftDoinker.set(false);
  RightDoinker.set(false);

  // Curve to eat all the doinkered rings + the one in a stack
  chassis.turn_to_angle(0);
  chassis.drive_distance(10, 0);
  chassis.right_swing_to_angle(180);
  chassis.drive_to_point(23.493, -47.191);

  // Eat two rings from the corner
  chassis.turn_to_angle(90);
  chassis.drive_to_pose(62.507, -62.507, 135, 0.5, 0);
  vex::wait(300, msec);
  chassis.drive_distance(-15, 135);
  RightDoinker.set(true);
  chassis.drive_distance(10, 135);
  vex::wait(300, msec);

  // Clear out the corner and drop goal in
  chassis.turn_to_angle(0);
  RightDoinker.set(false);
  chassis.turn_to_point(66.544, -66.544, 180);
  Clamp.set(false);

  // Touch ladder
  chassis.drive_to_pose(11.771, -32.102, 303, 0.5, 0);
  ladyBrownAllianceStakeScore = true;
  vex::wait(100, sec);
}

// NOT TESTED
void Autonomous::negative_ring_rush()
{
  int reversed;

  if (this->allianceColor == vex::color::red)
  {
    reversed = 1;
    alliance = "red";
    chassis.set_coordinates(-50.788, 27.547, 72);
  }
  else
  {
    reversed = -1;
    alliance = "blue";
    chassis.set_coordinates(50.788, 27.547, 282);
  }

  cout << "negative ring rush auto started, initial position:" << endl;
  cout << chassis.get_X_position() << ", " << chassis.get_Y_position() << endl;

  // Pursuit *purePursuit = new Pursuit(12.75);

  // Rush for center rings
  intakeSort = true;
  LeftDoinker.set(true);
  chassis.drive_to_point(-10.1, 43.158);

  // Clamp goal
  mogoClampDelay = 800;
  mogoClamp = true;
  chassis.drive_to_pose(-23.773, 23.773, 0, 0.5, 0);

  // Eat rushed ring in doinker + one more
  LeftDoinker.set(false);
  chassis.drive_to_point(-9.868, 41.452);

  // Go to the corner and eat two rings
  chassis.turn_to_angle(270);
  chassis.drive_to_pose(-62.507, 62.507, 315, 0.5, 0);
  vex::wait(300, msec);
  chassis.drive_distance(-15, 315);
  chassis.drive_distance(10, 315);
  vex::wait(300, msec);

  // Eat stack in front of alliance stake, hopefully color sorting the first one
  chassis.turn_to_point(-43.041, -16.153, 0);
  intakeSort = false;
  ringStopped = false;
  intakeToLadyBrownAuton = true;
  chassis.drive_to_point(-43.041, -16.153);
  vex::wait(300, msec);

  // Stick out the doinker to swat away the ring in front of alliance stake, then turn to it
  RightDoinker.set(true);
  chassis.drive_to_point(-49, 0);
  chassis.turn_to_point(-69, 0, 0);
  RightDoinker.set(false);

  chassis.heading_max_voltage = 0;
  chassis.drive_distance(15, 270);
  chassis.drive_distance(-15, 270);
  chassis.heading_max_voltage = 12;

  intakeToLadyBrownAuton = false;
  ladyBrownAllianceStakeScore = true;
  vex::wait(1, sec);

  // Touch ladder
  chassis.drive_distance(-25, 270);
  ladyBrownAllianceStakeScore = false;
  chassis.turn_to_point(-23.821, 0, 0);
  ladyBrownAllianceStakeScore = true;
  vex::wait(100, sec);
}

// NOT TESTED
void Autonomous::positive_goal_rush()
{

  cout << "positive goal rush auto started, initial position:" << endl;
  cout << chassis.get_X_position() << ", " << chassis.get_Y_position() << endl;

  int reversed;
  if (this->allianceColor == vex::color::red)
  {
    reversed = -1;
    alliance = "red";
    chassis.set_coordinates(-47.538, -60.141, 66.54);
  }
  else
  {
    reversed = 1;
    alliance = "blue";
    chassis.set_coordinates(47.538, -34.141, 246.54);
  }
}

void Autonomous::auton_skills()
{
  intakeToLadyBrownAuton = false;

  chassis.set_coordinates(-61, 0, 90);

  cout << "skills auto started, initial position:" << endl;
  cout << chassis.get_X_position() << ", " << chassis.get_Y_position() << endl;

  alliance = "red";
}

DriveParams changeThingy(float drive_max_voltage, float drive_kp, float drive_ki, float drive_kd, float drive_starti)
{
  DriveParams driveParams;
  driveParams.set_max_voltage(drive_max_voltage).set_kp(drive_kp).set_ki(drive_ki).set_kd(drive_kd).set_starti(drive_starti);
  return driveParams;
}

void Autonomous::testing()
{
  chassis.set_coordinates(0, 0, 0);

  DriveParams driveParams;
  TurnParams turnParams;

  while (true)
  {
    if (Controller.ButtonA.pressing()) // Reset coordinates
      chassis.set_coordinates(0, 0, 0);
    else if (Controller.ButtonB.pressing()) // Go forwards
      chassis.turn_to_angle(180, turnParams);
    else if (Controller.ButtonX.pressing()) // Go backwards
      chassis.turn_to_angle(0, turnParams);
    else if (Controller.ButtonY.pressing())
    { // Print data
      cout << "kP: " << turnParams.turn_kp << ", kI: " << turnParams.turn_ki << ", kD: " << turnParams.turn_kd << ", settle error: " << turnParams.turn_settle_error << ", drive slew: " << driveParams.drive_slew << endl;
      cout << "X: " << chassis.get_X_position() << ", Y: " << chassis.get_Y_position() << ", Theta: " << chassis.get_absolute_heading() << endl;
    }
    // Change PID constants
    else if (Controller.ButtonUp.pressing())
    {
      driveParams.set_kp(driveParams.drive_kp + 0.1);
      cout << "kP: " << driveParams.drive_kp << endl;
      driveParams = changeThingy(12, driveParams.drive_kp, driveParams.drive_ki, driveParams.drive_kd, 0);
    }
    else if (Controller.ButtonDown.pressing())
    {
      driveParams.set_kp(driveParams.drive_kp - 0.1);
      cout << "kP: " << driveParams.drive_kp << endl;
      driveParams = changeThingy(12, driveParams.drive_kp, driveParams.drive_ki, driveParams.drive_kd, 0);
    }
    else if (Controller.ButtonRight.pressing())
    {
      driveParams.set_kd(driveParams.drive_kd + 0.1);
      cout << "kD: " << driveParams.drive_kd << endl;
      driveParams = changeThingy(12, driveParams.drive_kp, driveParams.drive_ki, driveParams.drive_kd, 0);
    }
    else if (Controller.ButtonLeft.pressing())
    {
      driveParams.set_kd(driveParams.drive_kd - 0.1);
      cout << "kD: " << driveParams.drive_kd << endl;
      driveParams = changeThingy(12, driveParams.drive_kp, driveParams.drive_ki, driveParams.drive_kd, 0);
    }
    else if (Controller.ButtonL1.pressing())
    {
      driveParams.set_ki(driveParams.drive_ki + 0.1);
      cout << "kI: " << driveParams.drive_ki << endl;
      driveParams = changeThingy(12, driveParams.drive_kp, driveParams.drive_ki, driveParams.drive_kd, 0);
    }
    else if (Controller.ButtonL2.pressing())
    {
      driveParams.set_ki(driveParams.drive_ki - 0.1);
      cout << "kI: " << driveParams.drive_ki << endl;
      driveParams = changeThingy(12, driveParams.drive_kp, driveParams.drive_ki, driveParams.drive_kd, 0);
    }
    vex::wait(100, vex::timeUnits::msec);
  }
  vex::wait(100, sec);
}