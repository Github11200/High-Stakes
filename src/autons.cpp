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

void auton_skills()
{
  pre_driver = true;
  intakeToFrogAuton = false;

  odom_constants();
  chassis.set_coordinates(-61, 0, 90);

  cout << "skills auto started, initial position:" << endl;
  cout << chassis.get_X_position() << ", " << chassis.get_Y_position() << endl;

  task colorSortingAutonTask = task(colorSortingAutonTaskWrapper);
  task intakeToFrogAutonTask = task(intakeToFrogAutonTaskWrapper);

  Pursuit *purePursuit = new Pursuit(12.75);

  // Score on first alliance stake
  intakeSort = true;
  vex::wait(0.5, vex::timeUnits::sec);
  intakeSort = false;
  Intake.stop();

  // Back away
  chassis.drive_to_point(-47.212, 0);
  chassis.turn_to_point(-47.212, -23.503, 180);

  // Clamp onto the goal
  chassis.heading_max_voltage = 0;
  chassis.drive_max_voltage = 6;
  chassis.drive_to_point(-47.212, -23.503);
  odom_constants();
  Clamp.set(true);
  vex::wait(100, msec);
  intakeSort = true;

  // Eat a couple of rings, end up facing pile of blue
  purePursuit->followPath(skills[0], 12.75, 10000, true, 17, 2, 0.8);

  // Grab two red rings from under blue rings, holding them both for frog
  intakeSort = false;
  intakeToFrogAuton = true;
  odom_constants();
  chassis.turn_to_point(58.962, -47.269);
  chassis.drive_to_point(58.962, -47.269);

  // Align to wall stake
  chassis.turn_to_point(0, -47.269, 180);
  chassis.drive_to_point(0, -47.269);

  // Turn to the actual wall stake position, then drive forward
  chassis.turn_to_point(0, -69.388);
  chassis.drive_timeout = 600;
  chassis.drive_distance(14);
  default_constants();

  // Score twice on wall stake
  FrogMech.set(true);
  vex::wait(1000, msec);
  FrogMech.set(false);
  vex::wait(1000, msec);
  intakeToFrogAuton = true;
  int frogTimeout = 4000;
  while (intakeToFrogAuton && (frogTimeout >= 0))
  {
    wait(50, msec);
    frogTimeout -= 50;
  }
  intakeToFrogAuton = false;
  FrogMech.set(true);
  vex::wait(1000, msec);
  FrogMech.set(false);

  // Clean up the rest of the rings in the corner
  chassis.drive_to_point(0, -47.269);
  chassis.turn_to_point(-100, -47.269);
  intakeSort = true;
  purePursuit->followPath(skills[1], 12.75, 10000, true, 17, 3.5, 0.8);

  // Drop the goal in the corner
  vex::wait(100, msec);
  intakeSort = false;
  odom_constants();
  Clamp.set(false);
  chassis.drive_to_point(-51.979, -62.038);

  // Drive across the entire field
  intakeToFrogAuton = true;
  chassis.drive_to_point(47.141, -30.852);
  chassis.turn_to_point(47.141, 0, 180);
  Intake.stop();
  chassis.drive_to_point(47.141, -16);

  // Clamp the far goal
  chassis.drive_max_voltage = 6;
  chassis.drive_to_point(47.141, 0);
  odom_constants();
  Clamp.set(true);
  vex::wait(100, msec);
  intakeSort = true;

  // Go through the ladder and clean up the 2nd red corner.
  purePursuit->followPath(skills[2], 15, 10000, true, 17, 3, 0.8);
  odom_constants();
  intakeSort = false;
  Clamp.set(false);
  chassis.drive_to_point(-60.918, 54.716);

  // Clamp onto the final fillable goal
  chassis.turn_to_point(-47.141, 42);
  chassis.drive_to_point(-47.141, 42);
  chassis.turn_to_point(-47.141, 28, 180);
  chassis.drive_max_voltage = 6;
  chassis.drive_to_point(-47.141, 28);
  odom_constants();
  Clamp.set(true);
  vex::wait(100, msec);
  intakeSort = true;

  // Pick up one ring for frog
  chassis.turn_to_point(-28.54, 43.239);
  chassis.drive_to_point(-28.54, 43.239);
  intakeToFrogAuton = true;

  // Align in front of wall stake
  chassis.turn_to_point(0, 42.202);
  chassis.drive_to_point(0, 42.202);

  // Turn to the actual wall stake position, then drive forward
  chassis.turn_to_point(0, 69.388);
  chassis.drive_timeout = 1000;
  chassis.drive_distance(26);
  odom_constants();

  // Score once on 2nd wall stake
  FrogMech.set(true);
  vex::wait(1000, msec);
  FrogMech.set(false);

  intakeSort = true;
  chassis.drive_to_point(0, 53.127);
  chassis.turn_to_angle(90);

  // Eat two rings
  purePursuit->followPath(skills[3], 15, 10000, true, 17, 4, 0.8);
  odom_constants();

  // Eat rings under blue rings in the corner
  chassis.turn_to_point(45.5, 45.5);
  chassis.drive_to_point(45.5, 45.5);
  chassis.turn_to_point(46.77, 57.867);
  chassis.drive_to_point(46.77, 57.867);
  chassis.turn_to_point(45.5, 45.5, 180);
  chassis.drive_to_point(45.5, 45.5);
  chassis.turn_to_point(57.867, 46.77);
  intakeSort = false;
  intakeToFrogAuton = true;
  chassis.drive_to_point(57.867, 46.77);

  // Put last full mogo in corner
  chassis.turn_to_point(60.449, 55.51, 180);
  vex::wait(300, msec);
  Clamp.set(false);
  chassis.drive_to_point(60.449, 55.51);

  // Align to alliance stake
  purePursuit->followPath(skills[4], 15, 10000, true, 17, 4, 0.8);

  // Turn to the actual alliance stake position
  chassis.turn_to_point(69.388, 0, 180);
  chassis.drive_timeout = 1000;
  chassis.drive_distance(-24);
  odom_constants();

  // Score on alliance stake
  intakeSort = true;
  vex::wait(0.5, vex::timeUnits::sec);
  intakeSort = false;
  Intake.stop();

  // Push last goal into corner
  chassis.drive_to_point(54.086, 0);
  chassis.turn_to_point(60.449, -55.733, 180);
  chassis.drive_timeout = 1000;
  chassis.drive_to_point(60.449, -55.733);
  odom_constants();

  chassis.drive_distance(20);

  cout << "Final X: " << chassis.get_X_position() << endl;
  cout << "Final Y: " << chassis.get_Y_position() << endl;
}