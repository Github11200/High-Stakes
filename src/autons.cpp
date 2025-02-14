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

void fast_exit_conditions()
{
  chassis.set_drive_exit_conditions(2, 0.01, 4000);
  chassis.set_turn_exit_conditions(2, 0.01, 800);
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
  vex::wait(0.3, vex::timeUnits::sec);
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

  chassis.turn_to_angle(0);
  chassis.drive_distance(18, 0);
  chassis.drive_distance(-10, 0);
  odom_constants();

  // 12.75 and 8 before
  purePursuit->followPath(negativeRingRush[1], 15, 10000, false, 20, 11, 0.8);

  // Eat the third ring
  chassis.turn_to_point(-23.456 * reversed, 47.051);
  chassis.drive_to_point(-23.456 * reversed, 47.051);

  // For elims, get 1 more ring
  // chassis.turn_to_point(-43.637 * reversed, -3.44);
  // chassis.drive_to_point(-43.637 * reversed, -3.44);

  // Touch ladder
  chassis.turn_to_point(-23.773 * reversed, 0);
  chassis.drive_max_voltage = 12;
  chassis.drive_to_point(-23.773 * reversed, 0);
  chassis.drive_to_point(-23.773 * reversed, 0);
  chassis.drive_to_point(-23.773 * reversed, 0);
  Left.stop(vex::brakeType::brake);
  Right.stop(vex::brakeType::brake);
}

// NOT TESTED
void positive_alliance_stake_rush(string c)
{
  pre_driver = true;

  odom_constants();

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

  Pursuit *purePursuit = new Pursuit(12.75);

  // Get the goal
  chassis.drive_max_voltage = 5;
  if (alliance == "blue")
  {
    chassis.drive_distance(-28, 90);
  }
  else
  {
    chassis.drive_distance(-28, 270);
  }
  wait(100, vex::timeUnits::msec);
  Clamp.set(true);
  wait(500, vex::timeUnits::msec);

  odom_constants();

  // Get one ring
  chassis.turn_to_point(23.467 * reversed, -47.118);
  intakeSort = true;
  chassis.drive_to_point(23.467 * reversed, -47.118);
  wait(500, vex::timeUnits::msec);

  // Put the mogo near the positive corner
  chassis.turn_to_angle(0);
  wait(300, vex::timeUnits::msec);
  Clamp.set(false);
  wait(300, vex::timeUnits::msec);
  intakeSort = false;

  chassis.drive_distance(5, 0);

  if (alliance == "blue")
  {
    chassis.turn_to_angle(90);
  }
  else
  {
    chassis.turn_to_angle(270);
  }
  chassis.drive_timeout = 1000;
  chassis.heading_max_voltage = 0;
  chassis.drive_distance(-5);
  chassis.drive_max_voltage = 0;
  Left.stop(vex::brakeType::coast);
  Right.stop(vex::brakeType::coast);
  vex::wait(100, sec);
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
  task intakeToFrogAutonTask = task(intakeToFrogAutonTaskWrapper);
  int reversed;
  if (c == "red")
  {
    reversed = -1;
    alliance = "red";
    chassis.set_coordinates(-47.538, -60.141, 115);
  }
  else
  {
    reversed = 1;
    alliance = "blue";
    chassis.set_coordinates(47.538, -34.141, 245);
  }

  Pursuit *purePursuit = new Pursuit(12.75);

  // Rush to the goal
  Doinker.set(true);
  intakeToFrogAuton = true;
  if (alliance == "blue")
  {
    // Goal rush
    fast_exit_conditions();
    chassis.drive_distance(30.4, 245);
    chassis.drive_distance(-30.4, 245);

    odom_constants();
    // Turn the goal so that its position is consistent and it's away from the other goal
    chassis.turn_to_angle(225);

    // Let go of the goal and move back to make sure it's free
    Doinker.set(false);
    chassis.drive_distance(-5);

    // Turn around to clamp the goal
    chassis.turn_to_angle(70); // Tune this value to clamp on the goal
  }
  else
  {
    fast_exit_conditions();
    chassis.drive_distance(30.4, 115);
    chassis.drive_distance(-30.4, 115);

    odom_constants();
    // Turn the goal so that its position is consistent and it's away from the other goal
    chassis.turn_to_angle(225);

    // Let go of the goal and move back to make sure it's free
    Doinker.set(false);
    chassis.drive_distance(-5);

    // Turn around to clamp the goal
    chassis.turn_to_angle(70); // Tune this value to clamp on the goal
  }

  // Clamp the goal and score a ring
  chassis.drive_max_voltage = 6;
  chassis.drive_distance(-10);
  Clamp.set(true);
  vex::wait(100, msec);
  intakeSort = true;
  vex::wait(1000, msec);
  Clamp.set(false);
  chassis.drive_distance(10);

  // Clamp the 2nd goal
  chassis.turn_to_point(23.701 * reversed, -23.701, 180);
  chassis.drive_max_voltage = 6;
  chassis.drive_to_point(23.701 * reversed, -23.701);
  Clamp.set(true);
  intakeSort = true;
  odom_constants();

  // Touch the ladder
  chassis.turn_to_point(0 * reversed, -23.701);
  chassis.drive_timeout = 600;
  chassis.drive_to_point(0 * reversed, -23.701);
  chassis.drive_to_point(0 * reversed, -23.701);
}

void testing(string c)
{
  chassis.set_coordinates(0, 0, 90);
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

  while (true)
  {
    cout << chassis.get_X_position() << ", " << chassis.get_Y_position() << endl;
    vex::wait(1, sec);
  }
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
  vex::wait(0.3, vex::timeUnits::sec);
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

  // Grab two red rings from under blue rings, holding one for frog
  fast_exit_conditions();
  chassis.turn_to_point(58.962, -47.269);
  chassis.drive_timeout = 800;
  chassis.drive_to_point(58.962, -47.269);
  intakeSort = false;
  intakeToFrogAuton = true;
  odom_constants();

  // Align to wall stake
  chassis.turn_to_point(0, -38.202, 180);
  chassis.drive_to_point(6, -38.202); // wtf why is this 6 and why does this work ????

  // Turn to the actual wall stake position, then drive forward
  // chassis.turn_to_point(2, -69.388);
  chassis.turn_to_point(0, -69.388);
  chassis.drive_timeout = 600;
  chassis.drive_distance(26);
  // chassis.turn_to_point(0, -69.388);
  default_constants();

  // Score twice on wall stake
  FrogMech.set(true);
  vex::wait(700, msec);
  FrogMech.set(false);
  intakeToFrogAuton = true;
  int frogTimeout = 3000;
  while (intakeToFrogAuton && (frogTimeout >= 0))
  {
    wait(50, msec);
    frogTimeout -= 50;
  }
  wait(200, vex::timeUnits::msec);
  intakeToFrogAuton = false;
  FrogMech.set(true);
  vex::wait(700, msec);
  FrogMech.set(false);

  // Clean up the rest of the rings in the corner
  chassis.drive_to_point(0, -47.269);
  chassis.turn_to_point(-100, -47.269);
  intakeSort = true;
  purePursuit->followPath(skills[1], 12.75, 10000, true, 17, 3.5, 0.8);

  // Drop the goal in the corner
  intakeSort = false;
  vex::wait(100, msec);
  fast_exit_conditions();
  Clamp.set(false);
  chassis.drive_timeout = 600;
  chassis.drive_distance(-10);
  chassis.drive_timeout = 4000;

  // Eat the ring in front of ladder
  chassis.turn_to_point(1.576, -46.908);
  intakeToFrogAuton = true;
  chassis.drive_to_point(1.576, -46.908);
  // chassis.turn_to_point(23.764, -23.764); don't think i need this one, time save
  chassis.drive_to_point(23.764, -23.764);

  // Clamp the far goal
  odom_constants();
  chassis.turn_to_point(47.141, 0, 180);
  chassis.drive_max_voltage = 6;
  chassis.heading_max_voltage = 6;
  chassis.drive_to_point(47.141, 0);
  odom_constants();
  Clamp.set(true);
  vex::wait(100, msec);
  intakeSort = true;

  // Line up in front of the ladder
  fast_exit_conditions();
  chassis.turn_to_point(23.764, -23.764);
  chassis.drive_to_point(23.764, -23.764);
  intakeSort = false;

  // Go through the ladder
  // purePursuit->followPath(skills[2], 15, 10000, true, 17, 3, 0.8);
  chassis.turn_to_point(-23.764, 23.764);
  chassis.drive_to_point(0, 0);
  Intake.stop();
  chassis.drive_to_point(-23.764, 23.764);
  vex::wait(200, msec);

  // Eat up the two stored rings, and eat the corner of the corner rings
  intakeSort = true;
  chassis.drive_to_point(-44.1, 44.1);

  // Back up and eat a ring x2
  chassis.turn_to_point(-56.571, 45.86);
  chassis.drive_to_point(-56.571, 45.86);
  chassis.turn_to_point(-42.6, 42.6, 180);
  chassis.drive_to_point(-42.6, 42.6);
  chassis.turn_to_point(-45.86, 56.571);
  chassis.drive_to_point(-45.86, 56.571);

  // Put the goal in the corner
  chassis.turn_to_point(-54.7, 61, 180);
  intakeSort = false;
  chassis.drive_timeout = 600;
  Clamp.set(false);
  chassis.drive_distance(-18);

  // Clamp onto the final fillable goal
  chassis.turn_to_point(-47.141, 48);
  chassis.drive_to_point(-47.141, 48);
  odom_constants();
  chassis.turn_to_point(-47.141, 23.574, 180);
  chassis.drive_max_voltage = 6;
  chassis.drive_to_point(-47.141, 23.574);
  Clamp.set(true);
  vex::wait(100, msec);
  intakeSort = true;

  // Pick up one ring for frog
  chassis.turn_to_point(-23.503, 46.931);
  chassis.drive_to_point(-23.503, 46.931);
  intakeSort = false;
  intakeToFrogAuton = true;

  // Align in front of wall stake
  chassis.turn_to_point(0, 41.202); // added 3 cus its crashed into ladder
  chassis.drive_to_point(0, 41.202);

  // Turn to the actual wall stake position, then drive forward
  chassis.turn_to_point(0, 69.388);
  chassis.drive_timeout = 1000;
  chassis.drive_distance(26);
  // chassis.turn_to_point(0, 69.388);

  // Score once on 2nd wall stake
  FrogMech.set(true);
  vex::wait(700, msec);
  FrogMech.set(false);
  intakeToFrogAuton = true;
  frogTimeout = 3000;
  while (intakeToFrogAuton && (frogTimeout >= 0))
  {
    wait(50, msec);
    frogTimeout -= 50;
  }
  wait(200, vex::timeUnits::msec);
  intakeToFrogAuton = false;
  FrogMech.set(true);
  vex::wait(700, msec);
  FrogMech.set(false);

  // Eat two rings before the blue corner
  // purePursuit->followPath(skills[3], 15, 10000, true, 17, 4, 0.8);
  intakeSort = true;
  fast_exit_conditions();
  chassis.drive_to_point(0, 53.127);
  chassis.turn_to_point(23.381, 47.007);
  chassis.drive_to_point(23.381, 47.007);

  chassis.turn_to_point(23.381, 23.481);
  chassis.drive_to_point(23.381, 23.481);

  // First ring is the corner one in the triangle
  chassis.turn_to_point(44.1, 44.1);
  chassis.drive_to_point(44.1, 44.1);

  // Back up and eat a ring x2
  chassis.drive_timeout = 900;
  chassis.turn_to_point(45.86, 56.571);
  chassis.drive_to_point(45.86, 56.571);
  chassis.turn_to_point(42.6, 42.6, 180);
  chassis.drive_to_point(42.6, 42.6);
  chassis.turn_to_point(56.571, 45.86);
  chassis.drive_to_point(56.571, 45.86);

  // Doinker out the corner
  Doinker.set(true);
  chassis.turn_to_point(60.449, 56.51);
  chassis.drive_timeout = 600;
  chassis.drive_distance(15);
  chassis.turn_timeout = 400;
  chassis.turn_to_angle(270);
  fast_exit_conditions();

  // Put last full mogo in corner
  chassis.turn_to_point(60.449, 56.51, 180);
  Clamp.set(false);
  Doinker.set(false);
  chassis.drive_timeout = 400;
  chassis.drive_distance(-16.5, chassis.get_absolute_heading());

  // Get out of the corner
  odom_constants();
  chassis.drive_distance(5);

  // Clamp the final goal
  chassis.turn_to_point(59.149, 23.481, 180);
  chassis.drive_to_point(59.149, 23.481, 0, 6, 6);
  Clamp.set(true);
  vex::wait(100, msec);

  // Doinker and path to the last corner
  chassis.turn_to_angle(210);
  Doinker.set(true);
  purePursuit->followPath(skills[4], 15, 10000, true, 17, 4, 0.8);

  chassis.turn_to_angle(0);
  chassis.drive_timeout = 700;
  chassis.turn_to_point(65.449, -55.733, 180);
  Doinker.set(false);
  Clamp.set(false);
  chassis.drive_distance(-18);
  chassis.drive_distance(10);

  // // Push last goal into corner
  // chassis.drive_timeout = 4000;
  // chassis.drive_to_point(65.449, -55.733);
  // odom_constants();

  // chassis.drive_distance(20);

  // cout << "Final X: " << chassis.get_X_position() << endl;
  // cout << "Final Y: " << chassis.get_Y_position() << endl;
}