#include "vex.h"
#include "../include/driver/intake.h"
#include "../include/driver/ladyBrown.h"
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
float ladyBrownDelay = 0;
IntakeControl intakeControl(12, 3, OpticalSensor.hue());
LadyBrown ladyBrown(20, 180, 220, 12, 2, 3, 5, 0.1, 2000, 10000000, 270, 0);

vector<Point> mirrorPath(vector<Point> originalPath)
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

int intakeAutonTaskWrapper()
{
  while (true)
  {
    intakeControl.intakeAutonTask();
    vex::wait(50, vex::timeUnits::msec);
  }
  return 1;
}

int ladyBrownAutonTaskWrapper()
{
  while (true)
  {
    ladyBrown.ladyBrownAutonTask();
    vex::wait(50, vex::timeUnits::msec);
  }
  return 1;
}

// NOT TESTED
void negative_alliance_stake_rush(string c)
{
  pre_driver = true;

  cout << "negative alliance stake auto started, initial position:" << endl;
  cout << chassis.get_X_position() << ", " << chassis.get_Y_position() << endl;

  task intakeAutonTask = task(intakeAutonTaskWrapper);
  int reversed;

  if (c == "red")
  {
    reversed = 1;
    alliance = "red";
    chassis.set_coordinates(-56.35, 20.793, 0);
  }
  else
  {
    reversed = -1;
    alliance = "blue";
    chassis.set_coordinates(56.35, 20.793, 0);
    for (int i = 0; i < negativeRingRush.size(); ++i)
      negativeRingRush[i] = mirrorPath(negativeRingRush[i]);
  }
}

// NOT TESTED
void positive_alliance_stake_rush(string c)
{
  pre_driver = true;

  task intakeAutonTask = task(intakeAutonTaskWrapper);
  int reversed;
  if (c == "red")
  {
    chassis.set_coordinates(-50.197, -23.622, 270);
    reversed = -1;
    alliance = "red";
  }
  else
  {
    chassis.set_coordinates(50.197, -23.622, 90);
    reversed = 1;
    alliance = "blue";
  }

  cout << "positive alliance stake auto started, initial position:" << endl;
  cout << chassis.get_X_position() << ", " << chassis.get_Y_position() << ", " << chassis.get_absolute_heading() << endl;
}

// NOT TESTED
void negative_ring_rush(string c)
{
  pre_driver = true;

  cout << "negative ring rush auto started, initial position:" << endl;
  cout << chassis.get_X_position() << ", " << chassis.get_Y_position() << endl;

  task intakeAutonTask = task(intakeAutonTaskWrapper);
  task ladyBrownAutonTask = task(ladyBrownAutonTaskWrapper);
  int reversed;
  if (c == "red")
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

  // Pursuit *purePursuit = new Pursuit(12.75);

  // Rush for center rings
  intakeSort = true;
  LeftDoinker.set(true);
  chassis.drive_to_point(-10.1, 43.158);

  // Clamp goal
  chassis.drive_to_pose(-23.773, 23.773, 0, 0.5, 0);
  Clamp.set(true);

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
  vex::wait(100, sec);
}

// NOT TESTED
void positive_goal_rush(string c)
{
  pre_driver = true;

  cout << "positive goal rush auto started, initial position:" << endl;
  cout << chassis.get_X_position() << ", " << chassis.get_Y_position() << endl;

  task intakeAutonTask = task(intakeAutonTaskWrapper);
  int reversed;
  if (c == "red")
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

void testing(string c)
{
  chassis.set_coordinates(0, 0, 0);
  cout << "testing auto started, initial position:" << endl;
  cout << chassis.get_X_position() << ", " << chassis.get_Y_position() << endl;
  pre_driver = true;

  task intakeAutonTask = task(intakeAutonTaskWrapper);
  int reversed;
  float kP = 0.18;
  float kI = 0.02;
  float kD = 1;
  float settle_error = 1;

  if (c == "red")
  {
    reversed = -1;
    alliance = "red";
  }
  else if (c == "blue")
  {
    reversed = 1;
    alliance = "blue";
  }
}

void auton_skills()
{
  pre_driver = true;
  intakeToLadyBrownAuton = false;

  chassis.set_coordinates(-61, 0, 90);

  cout << "skills auto started, initial position:" << endl;
  cout << chassis.get_X_position() << ", " << chassis.get_Y_position() << endl;

  task intakeAutonTask = task(intakeAutonTaskWrapper);

  alliance = "red";
}