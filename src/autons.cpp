#include "vex.h"
#include "../include/driver/intake.h"
#include "../include/auto/skills.h"

using namespace vex;
using namespace std;

bool intakeToFrogAuton = false;
bool intakeSort = false;
bool intakeRev = false;
bool raiseFrog = false;
IntakeControl intakeControl(12, 3, OpticalSensor.hue());

void default_constants()
{
  chassis.set_drive_constants(12, 0.75, 0.01, 1.7, 3);
  chassis.set_heading_constants(12, 0.28, 0.1, 1.9, 1);
  chassis.set_turn_constants(12, 0.28, 0.1, 1.9, 1);
  chassis.set_swing_constants(12, 0.3, 0.01, 2, 3);

  chassis.set_drive_exit_conditions(1.5, 100, 4000);
  chassis.set_turn_exit_conditions(1.5, 100, 800);
  chassis.set_swing_exit_conditions(1, 5000, 5000);
}

void odom_constants()
{
  default_constants();
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

// NOT TESTED WITH CURRENT PID
void negative_alliance_stake_rush(string c)
{
  pre_driver = true;

  task colorSortingAutonTask = task(colorSortingAutonTaskWrapper);
  int reversed;
  if (c == "blue")
  {
    reversed = -1;
    alliance = "blue";
  }
  else if (c == "red")
  {
    reversed = 1;
    alliance = "red";
  }
  odom_constants();
}

// DONE AND TESTED WITH CURRENT PID
void positive_alliance_stake_rush(string c)
{
  pre_driver = true;

  task colorSortingAutonTask = task(colorSortingAutonTaskWrapper);

  int reversed;
  if (c == "blue")
  {
    reversed = -1;
  }
  else if (c == "red")
  {
    reversed = 1;
  }
  odom_constants();
}

// NOT TESTED WITH CURRENT PID
void negative_ring_rush(string c)
{
  pre_driver = true;

  task colorSortingAutonTask = task(colorSortingAutonTaskWrapper);
  int reversed;
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
  odom_constants();
}

// NOT TESTED WITH CURRENT PID
void positive_corner_rush(string c)
{
  pre_driver = true;

  task colorSortingAutonTask = task(colorSortingAutonTaskWrapper);
  int reversed;
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
  odom_constants();
}

void testing(string c)
{
  cout << "testing auto started" << endl;
  cout << chassis.get_X_position() << ", " << chassis.get_Y_position() << endl;
  pre_driver = true;

  task colorSortingAutonTask = task(colorSortingAutonTaskWrapper);
  int reversed;
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
  odom_constants();
  cout << "constants defined" << endl;

  Pursuit *purePursuit = new Pursuit(12.75);
  purePursuit->followPath(skills.at(0), 12.75, 10000, true, 0.3, 0, 0);
}

// Part after 2nd wall stake not tested with current PID
void auton_skills()
{
  pre_driver = true;

  odom_constants();

  task colorSortingAutonTask = task(colorSortingAutonTaskWrapper);
  task intakeToFrogAutonTask = task(intakeToFrogAutonTaskWrapper);

  intakeSort = true;
  vex::wait(0.5, vex::timeUnits::sec);
  intakeSort = false;
}