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
bool raiseFrog = false;
IntakeControl intakeControl(12, 3, OpticalSensor.hue());

vector<Point> mirrorPath(vector<Point> originalPath)
{
  for (int i = 0; i < originalPath.size(); ++i)
    originalPath[i].x = -originalPath[i].x;
  return originalPath;
}

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

// NOT TESTED
void negative_alliance_stake_rush(string c)
{
  pre_driver = true;

  odom_constants();

  cout << "negative alliance stake auto started, initial position:" << endl;
  cout << chassis.get_X_position() << ", " << chassis.get_Y_position() << endl;

  task colorSortingAutonTask = task(colorSortingAutonTaskWrapper);
  int reversed;

  if (c == "red")
  {
    reversed = 1;
    alliance = "red";
    chassis.set_coordinates(-56.35, 20.793, 0);
    cout << "RED" << endl;
  }
  else
  {
    reversed = -1;
    alliance = "blue";
    chassis.set_coordinates(56.35, 20.793, 0);
    for (int i = 0; i < negativeRingRush.size(); ++i)
      negativeRingRush[i] = mirrorPath(negativeRingRush[i]);
  }

  Pursuit *purePursuit = new Pursuit(12.75);

  // Push rings out of the way in front of alliance stake
  chassis.drive_to_point(-56.35 * reversed, 0);

  // Score on alliance stake
  chassis.turn_to_point(-69 * reversed, 0, 180);
  chassis.drive_timeout = 600;
  chassis.drive_distance(-7, chassis.get_absolute_heading());
  odom_constants();
  intakeSort = true;
  vex::wait(0.5, vex::timeUnits::sec);
  intakeSort = false;
  Intake.stop();

  // Drive away from alliance stake, getting in line with mogo
  chassis.turn_to_point(-42.644 * reversed, 12.252);
  chassis.drive_to_point(-42.644 * reversed, 12.252);

  // Clamp mogo
  chassis.turn_to_point(-22.535 * reversed, 26, 180);
  chassis.drive_max_voltage = 6;
  chassis.drive_to_point(-22.535 * reversed, 26);
  Clamp.set(true);
  vex::wait(100, msec);

  intakeSort = true;

  // Turn towards the first point in the path and then follow the path to get the 2 rings from the 8 stack
  chassis.turn_to_point(negativeRingRush[0][3].x, negativeRingRush[0][3].y);
  purePursuit->followPath(negativeRingRush[0], 12.75, 10000, true, 17, 3, 0.8);

  chassis.drive_distance(18, 0);
  chassis.drive_distance(-10, 0);
  odom_constants();

  purePursuit->followPath(negativeRingRush[1], 12.75, 10000, false, 17, 3, 0.8);

  // Eat the third ring
  chassis.turn_to_point(-23.456 * reversed, 47.051);
  chassis.drive_to_point(-23.456 * reversed, 47.051);

  wait(500, vex::timeUnits::msec);

  // For elims, get 1 more ring
  // chassis.turn_to_point(-43.637 * reversed, -3.44);
  // chassis.drive_to_point(-43.637 * reversed, -3.44);

  // Touch ladder
  chassis.turn_to_point(-23.773 * reversed, 0);
  chassis.drive_max_voltage = 6;
  chassis.drive_to_point(-23.773 * reversed, 0);
  chassis.drive_to_point(-23.773 * reversed, 0);
  chassis.drive_to_point(-23.773 * reversed, 0);
}

// NOT TESTED
void positive_alliance_stake_rush(string c)
{
  pre_driver = true;

  odom_constants();

  cout << "positive alliance stake auto started, initial position:" << endl;
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
  }

  Pursuit *purePursuit = new Pursuit(12.75);
}

// NOT TESTED
void negative_ring_rush(string c)
{
  pre_driver = true;

  odom_constants();

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

  // Move back and clamp the mogo
  chassis.drive_max_voltage = 6;
  chassis.drive_to_point(-31 * reversed, 23.556);
  Clamp.set(true);
  vex::wait(100, msec);

  intakeSort = true;

  // Turn towards the first point in the path and then follow the path to get the 2 rings from the 8 stack
  chassis.turn_to_point(-16.786 * reversed, 30.027);
  if (alliance == "red")
    purePursuit->followPath(negativeRingRush[0], 12.75, 10000, true, 17, 3, 0.8);
  else
    vex::wait(500, msec); // Replace with mirrored path on x-axis

  // Follow the path again but backwards
  std::reverse(negativeRingRush[0].begin(), negativeRingRush[0].end());

  if (alliance == "red")
    purePursuit->followPath(negativeRingRush[0], 12.75, 10000, false, 17, 3, 0.8);
  else
    vex::wait(500, msec); // Replace with mirrored path on x-axis

  // Eat the third ring
  chassis.turn_to_point(-23.456, 47.051);
  chassis.drive_to_point(-23.456, 47.051);

  // Turn to the first point in the second path
  chassis.turn_to_point(negativeRingRush[1][0].x, negativeRingRush[1][0].y);

  // Follow the second path to the corner and intake the ring in the stack (hopefully)
  purePursuit->followPath(negativeRingRush[1], 12.75, 10000, true, 17, 3, 0.8);
  vex::wait(500, vex::timeUnits::msec);

  // Back away
  chassis.drive_to_point(-47.141, 26.753);

  // Intake the stack, hopefully color sorting the first one
  chassis.turn_to_point(-47.141, -14.564);
  chassis.drive_to_point(-47.141, -14.564);

  // Touch ladder
  chassis.turn_to_point(-23.773, 0);
  chassis.drive_timeout = 600;
  chassis.drive_to_point(-23.773, 0);
  chassis.drive_to_point(-23.773, 0);
}

// NOT TESTED
void positive_goal_rush(string c)
{
  pre_driver = true;

  odom_constants();

  cout << "positive goal rush auto started, initial position:" << endl;
  cout << chassis.get_X_position() << ", " << chassis.get_Y_position() << endl;

  task colorSortingAutonTask = task(colorSortingAutonTaskWrapper);
  int reversed;
  if (c == "red")
  {
    reversed = 1;
    alliance = "red";
    chassis.set_coordinates(-50, -60, 0);
  }
  else
  {
    reversed = -1;
    alliance = "blue";
    chassis.set_coordinates(50, -60, 0);
  }

  Pursuit *purePursuit = new Pursuit(12.75);

  // Rush to the goal
  if (alliance == "red")
    purePursuit->followPath(positiveGoalRush[0], 12.75, 10000, true, 17, 5, 0.8);
  else
    purePursuit->followPath(positiveGoalRush[1], 12.75, 10000, true, 17, 5, 0.8);
  Doinker.set(true);

  // Come back
  std::reverse(positiveGoalRush[0].begin(), positiveGoalRush[0].end());
  if (alliance == "red")
    purePursuit->followPath(positiveGoalRush[0], 12.75, 10000, false, 17, 5, 0.8);
  else
    purePursuit->followPath(positiveGoalRush[1], 12.75, 10000, false, 17, 5, 0.8);

  // Drag the goal in the middle of the tiles before letting go
  if (alliance == "red")
    chassis.turn_to_angle(60);
  else
    chassis.turn_to_angle(240);
  Doinker.set(false);

  // Turn around and clamp the goal
  chassis.turn_to_point(0, -60, 180);
  chassis.drive_max_voltage = 6;
  chassis.drive_to_point(35.818, -60);
  Clamp.set(true);
  intakeSort = true;
  odom_constants();

  // Eat a ring, but save it for the other goal
  chassis.turn_to_point(23.701, -47.141);
  chassis.drive_to_point(35.818, -60);
  Clamp.set(false);
  intakeSort = false;

  // Clamp the 2nd goal
  chassis.turn_to_point(23.701, -23.701, 180);
  chassis.drive_max_voltage = 6;
  chassis.drive_to_point(23.701, -23.701);
  Clamp.set(true);
  intakeSort = true;
  odom_constants();

  // Touch the ladder
  chassis.turn_to_point(0, -23.701);
  chassis.drive_timeout = 600;
  chassis.drive_to_point(0, -23.701);
  chassis.drive_to_point(0, -23.701);
}

void testing(string c)
{
  cout << "testing auto started, initial position:" << endl;
  cout << chassis.get_X_position() << ", " << chassis.get_Y_position() << endl;
  pre_driver = true;

  task colorSortingAutonTask = task(colorSortingAutonTaskWrapper);
  task intakeToFrogAutonTask = task(intakeToFrogAutonTaskWrapper);
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

  intakeSort = false;
  intakeToFrogAuton = true;
  while (intakeToFrogAuton)
  {
    wait(10, vex::timeUnits::msec);
  }
  FrogMech.set(true);
  wait(700, vex::timeUnits::msec);
  wait(50, vex::timeUnits::sec);
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

  alliance = "red";

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
  purePursuit->followPath(skills[0], 12.75, 10000, true, 17, 3, 0.8);

  // Grab two red rings from under blue rings, holding them both for frog
  intakeSort = false;
  intakeToFrogAuton = true;
  odom_constants();
  chassis.turn_to_point(58.962, -47.269);
  chassis.drive_timeout = 800;
  chassis.drive_to_point(58.962, -47.269);
  odom_constants();

  // Align to wall stake
  chassis.turn_to_point(0, -47.269, 180);
  chassis.drive_to_point(6, -47.269);

  // Turn to the actual wall stake position, then drive forward
  chassis.turn_to_point(2, -69.388);
  chassis.drive_timeout = 600;
  chassis.drive_distance(18);
  chassis.turn_to_point(2, -69.388);
  default_constants();

  // Score twice on wall stake
  FrogMech.set(true);
  vex::wait(700, msec);
  FrogMech.set(false);
  vex::wait(700, msec);
  intakeToFrogAuton = true;
  int frogTimeout = 4000;
  while (intakeToFrogAuton && (frogTimeout >= 0))
  {
    wait(50, msec);
    frogTimeout -= 50;
  }
  wait(200, vex::timeUnits::msec);
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
  chassis.drive_timeout = 600;
  chassis.drive_distance(-10);
  odom_constants();

  // Drive across the entire field
  Hooks.stop(vex::brakeType::coast);
  Intake.spin(vex::directionType::fwd, 12, vex::voltageUnits::volt);
  chassis.drive_to_point(47.141, -30.852);
  chassis.turn_to_angle(180);

  // Clamp the far goal
  chassis.drive_max_voltage = 6;
  chassis.heading_max_voltage = 6;
  chassis.drive_distance(-30, 180);
  odom_constants();
  Clamp.set(true);
  vex::wait(100, msec);
  intakeSort = true;

  // Go through the ladder and clean up the 2nd red corner.
  purePursuit->followPath(skills[2], 15, 10000, true, 17, 3, 0.8);

  // Get the last ring in the triangle
  odom_constants();
  chassis.turn_to_point(-57.921, 41.268);
  chassis.drive_to_point(-57.921, 41.268);
  chassis.drive_distance(2);
  chassis.drive_distance(-2);

  odom_constants();
  intakeSort = false;
  chassis.turn_to_point(-60.918, 54.716, 180);
  chassis.drive_timeout = 600;
  Clamp.set(false);
  chassis.drive_distance(-18);
  odom_constants();

  // Clamp onto the final fillable goal
  chassis.turn_to_point(-47.141, 42);
  chassis.drive_to_point(-47.141, 42);
  chassis.turn_to_point(-47.141, 25, 180);
  chassis.drive_max_voltage = 6;
  chassis.drive_to_point(-47.141, 25);
  odom_constants();
  Clamp.set(true);
  vex::wait(100, msec);
  intakeSort = true;

  // Pick up one ring for frog
  chassis.turn_to_point(-28.54, 42.239);
  chassis.drive_to_point(-28.54, 42.239);

  // Align in front of wall stake
  chassis.turn_to_point(0, 38.202);
  chassis.drive_to_point(0, 38.202);

  // Turn to the actual wall stake position, then drive forward
  chassis.turn_to_point(0, 69.388);
  chassis.drive_timeout = 1000;
  intakeSort = false;
  intakeToFrogAuton = true;
  chassis.drive_distance(26, 0, 6, 6);
  chassis.turn_to_point(0, 69.388);
  odom_constants();

  // Score once on 2nd wall stake
  frogTimeout = 3000;
  while (intakeToFrogAuton && (frogTimeout >= 0))
  {
    wait(50, msec);
    frogTimeout -= 50;
  }
  vex::wait(300, vex::timeUnits::msec);
  FrogMech.set(true);
  vex::wait(1000, msec);
  FrogMech.set(false);

  intakeSort = true;
  chassis.drive_to_point(0, 53.127);
  chassis.turn_to_angle(110);

  // Eat two rings
  purePursuit->followPath(skills[3], 15, 10000, true, 17, 4, 0.8);
  odom_constants();
  intakeSort = false;
  intakeToFrogAuton = true;

  // Eat rings under blue rings in the corner

  // First ring is the corner one in the triangle
  chassis.turn_to_point(45.5, 45.5);
  intakeToFrogAuton = false;
  intakeSort = true;
  chassis.drive_to_point(45.5, 45.5, 0, 8, 8);

  // Then the one to the left of it
  chassis.drive_timeout = 900;
  chassis.turn_to_point(46.77, 56.867);
  chassis.drive_to_point(46.77, 56.867);

  // Move back
  chassis.turn_to_point(45.5, 45.5, 180);
  chassis.drive_to_point(45.5, 45.5);

  // Last ring in the corner
  chassis.turn_to_point(58.867, 49.77);
  chassis.drive_to_point(58.867, 49.77);

  // Put last full mogo in corner
  chassis.turn_to_point(60.449, 56.51, 180);
  chassis.drive_distance(5);
  Clamp.set(false);
  chassis.drive_timeout = 100;
  chassis.drive_distance(1.5);
  chassis.drive_timeout = 800;
  chassis.drive_distance(-16.5, chassis.get_absolute_heading(), 12, 12);
  odom_constants();

  chassis.drive_distance(2);
  chassis.turn_to_point(65.449, -55.733, 180);
  purePursuit->followPath(skills[4], 15, 10000, false, 17, 4, 0.8);

  chassis.drive_timeout = 700;
  chassis.drive_distance(-8);
  chassis.drive_distance(150);

  // // Push last goal into corner
  // chassis.drive_timeout = 4000;
  // chassis.drive_to_point(65.449, -55.733);
  // odom_constants();

  // chassis.drive_distance(20);

  // cout << "Final X: " << chassis.get_X_position() << endl;
  // cout << "Final Y: " << chassis.get_Y_position() << endl;
}