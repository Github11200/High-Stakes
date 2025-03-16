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

void default_constants()
{
  // Each constant set is in the form of (maxVoltage, kP, kI, kD, startI).
  chassis.set_drive_constants(12, 0.8, 0.01, 4.4, 3);
  chassis.set_heading_constants(10, 0.18, 0.02, 1, 1);
  chassis.set_turn_constants(10, 0.18, 0.02, 1, 1);
  chassis.set_swing_constants(12, 0.3, 0.01, 2, 3);

  // Each exit condition set is in the form of (settle_error, settle_time, timeout).
  chassis.set_drive_exit_conditions(1, 100, 8000);
  chassis.set_turn_exit_conditions(1, 100, 800);
  chassis.set_swing_exit_conditions(1, 5000, 5000);
}

void goal_constants()
{
  chassis.set_drive_constants(12, 0.7, 0.01, 3.5, 3);
  chassis.set_heading_constants(10, 0.15, 0.02, 1, 1);
}

void fast_exit_conditions()
{
  chassis.set_drive_exit_conditions(2, 0.01, 4000);
  chassis.set_turn_exit_conditions(2, 0.01, 800);
  chassis.set_swing_exit_conditions(1, 5000, 5000);
}

void exact_exit_conditions()
{
  chassis.set_drive_exit_conditions(0.5, 200, 4000);
  chassis.set_turn_exit_conditions(1, 200, 1200);
  chassis.set_swing_exit_conditions(1, 5000, 5000);
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

  default_constants();

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

  default_constants();

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

  default_constants();

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

  default_constants();

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
  cout << chassis.get_ForwardTracker_position() << endl;
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

  default_constants();
  chassis.set_coordinates(-61, 0, 90);

  cout << "skills auto started, initial position:" << endl;
  cout << chassis.get_X_position() << ", " << chassis.get_Y_position() << endl;

  task colorSortingAutonTask = task(colorSortingAutonTaskWrapper);
  task intakeToFrogAutonTask = task(intakeToFrogAutonTaskWrapper);

  alliance = "red";
}