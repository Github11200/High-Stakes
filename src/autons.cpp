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
  cout << "odom constants set" << endl;
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
  cout << "testing auto started, initial position:" << endl;
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

  Pursuit *purePursuit = new Pursuit(12.75);
  purePursuit->followPath(skills[0], 12.75, 10000, false, 17, 5, 0.8);
}

// Part after 2nd wall stake not tested with current PID
void auton_skills()
{
  pre_driver = true;
  intakeToFrogAuton = false;

  odom_constants();
  chassis.set_coordinates(-62, 0, 90);

  cout << "skills auto started, initial position:" << endl;
  cout << chassis.get_X_position() << ", " << chassis.get_Y_position() << endl;

  task colorSortingAutonTask = task(colorSortingAutonTaskWrapper);
  task intakeToFrogAutonTask = task(intakeToFrogAutonTaskWrapper);

  Pursuit *purePursuit = new Pursuit(12.75);

  intakeSort = true;
  vex::wait(0.5, vex::timeUnits::sec);
  intakeSort = false;

  chassis.drive_to_point(-47.212, 0);
  chassis.turn_to_point(-47.212, -23.503, 180);

  // Clamp onto the goal, using goal rush code tech
  chassis.heading_max_voltage = 0;
  chassis.drive_max_voltage = 6;
  chassis.drive_to_point(-47.212, -23.503);
  Clamp.set(true);
  chassis.drive_timeout = 4000;
  intakeSort = true;

  chassis.turn_to_point(skills[0][0].x, skills[0][0].y);
  // Eat a couple of rings, end up facing pile of blue
  purePursuit->followPath(skills[0], 12.75, 10000, true, 3, 5, 0.8);

  return;

  // Grab two red rings from under blue rings, holding them both for frog
  intakeSort = false;
  intakeToFrogAuton = true;
  chassis.drive_to_point(58.962, -47.269);

  // Score on wall stake
  chassis.drive_to_point(0, -47.269);
  chassis.turn_to_point(0, -59.059);
  chassis.drive_to_point(0, -59.059);

  FrogMech.set(true);
  vex::wait(150, msec);
  FrogMech.set(false);
  intakeToFrogAuton = true;
  vex::wait(500, msec);
  FrogMech.set(true);
  vex::wait(150, msec);
  FrogMech.set(false);

  // Clean up the rest of the rings in the corner
  chassis.drive_to_point(0, -47.269);
  chassis.turn_to_point(100, -47.269);
  purePursuit->followPath(skills[1], 12.75, 10000, false, 17, 5, 0.8);

  // Drop the goal in the corner
  Clamp.set(false);
  chassis.drive_to_point(-51.979, -62.038);

  // Push the mogo out of the way, then score on alliance stake
  purePursuit->followPath(skills[2], 12.75, 10000, false, 17, 5, 0.8);
  chassis.drive_to_point(47.141, 0);
  chassis.turn_to_angle(180);
  chassis.drive_to_point(-58, 0);
}