#include "vex.h"
#include "../include/driver/intake.h"
#include "../include/auto/skills.h"
#include "../include/auto/negativeRingRush.h"
#include "../include/auto/positiveGoalRush.h"

using namespace vex;
using namespace std;

bool intakeToFrogAuton = false;
bool intakeSort = false;
bool intakeRev = false;
IntakeControl intakeControl(12, 3, OpticalSensor.hue());

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

int colorSortingAutonTaskWrapper()
{
  while (true)
  {
    intakeControl.colorSortingAutonTask();
    vex::wait(50, vex::timeUnits::msec);
  }
  return 1;
}

int intakeToFrogAutonTaskWrapper()
{
  while (true)
  {
    intakeControl.intakeToFrogAutonTask();
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

  task colorSortingAutonTask = task(colorSortingAutonTaskWrapper);
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

  task colorSortingAutonTask = task(colorSortingAutonTaskWrapper);
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

  task colorSortingAutonTask = task(colorSortingAutonTaskWrapper);
  int reversed;
  if (c == "red")
  {
    reversed = 1;
    alliance = "red";
    chassis.set_coordinates(-50, 23.556, 270);
  }
  else
  {
    reversed = -1;
    alliance = "blue";
    chassis.set_coordinates(50, 23.556, 90);
    for (vector<Point> path : negativeRingRush)
      mirrorPath(path);
  }

  Pursuit *purePursuit = new Pursuit(12.75);
}

// NOT TESTED
void positive_goal_rush(string c)
{
  pre_driver = true;

  cout << "positive goal rush auto started, initial position:" << endl;
  cout << chassis.get_X_position() << ", " << chassis.get_Y_position() << endl;

  task colorSortingAutonTask = task(colorSortingAutonTaskWrapper);
  task intakeToFrogAutonTask = task(intakeToFrogAutonTaskWrapper);
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

  task colorSortingAutonTask = task(colorSortingAutonTaskWrapper);
  task intakeToFrogAutonTask = task(intakeToFrogAutonTaskWrapper);
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
  intakeToFrogAuton = false;

  chassis.set_coordinates(-61, 0, 90);

  cout << "skills auto started, initial position:" << endl;
  cout << chassis.get_X_position() << ", " << chassis.get_Y_position() << endl;

  task colorSortingAutonTask = task(colorSortingAutonTaskWrapper);
  task intakeToFrogAutonTask = task(intakeToFrogAutonTaskWrapper);

  alliance = "red";
}