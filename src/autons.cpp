#include "vex.h"
#include "../include/driver/intake.h"
#include "../include/driver/fishy.h"

using namespace vex;
using namespace std;

bool intakeToFishyAuton = false;
bool intakeSort = false;
bool raiseFishy = false;
IntakeControl intakeControl(12, 3, OpticalSensor.hue());
Fishy fishyControl(12);

void default_constants()
{
  chassis.set_drive_constants(10, 1.5, 0, 10, 0);
  chassis.set_heading_constants(6, .4, 0, 1, 0);
  chassis.set_turn_constants(12, .4, .03, 3, 15);
  chassis.set_swing_constants(12, .3, .001, 2, 15);
  chassis.set_drive_exit_conditions(1.5, 300, 5000);
  chassis.set_turn_exit_conditions(1, 300, 800);
  chassis.set_swing_exit_conditions(1, 300, 800);
}

void odom_constants()
{
  default_constants();
  chassis.drive_max_voltage = 8;
  chassis.drive_settle_error = 3;
}

// TESTED, BUT MOVED FROM ALLIANCE_NEGATIVE_BLUE TO ALLIANCE_NEGATIVE
void alliance_negative(std::string c)
{
  int reversed;
  int num;
  if (c == "red")
  {
    reversed = -1;
    num = 360;
  }
  else if (c == "blue")
  {
    reversed = 1;
    num = 0;
  }
  odom_constants();
  chassis.set_heading(num + (reversed * 90));

  // Get the Alliance Stake, pushing rings out of the way
  chassis.set_drive_exit_conditions(1.5, 300, 700);
  chassis.drive_distance(-9.5, num + (reversed * 90));
  chassis.set_turn_exit_conditions(1, 300, 300);
  chassis.turn_to_angle(180);
  chassis.set_drive_exit_conditions(1.5, 300, 300);
  chassis.drive_distance(-4, 180);
  Intake.spin(fwd, 12, volt);
  wait(600, msec);
  Intake.stop();

  // Move forwards
  chassis.set_drive_exit_conditions(1.5, 300, 700);
  chassis.drive_distance(15.5, 180);

  // Move backwards and clamp onto the goal
  chassis.set_turn_exit_conditions(1, 300, 500);
  chassis.turn_to_angle(num + (reversed * 315));
  chassis.set_drive_exit_conditions(1.5, 300, 800);
  chassis.drive_distance(-25, num + (reversed * 315));
  chassis.drive_distance(-5, num + (reversed * 315), 3, 3);
  Clamp.set(true);

  // Sweep the two rings on the edge of the line
  chassis.turn_to_angle(num + (reversed * 145));
  chassis.set_drive_exit_conditions(1.5, 300, 600);
  chassis.drive_distance(19, num + (reversed * 135));
  Intake.spin(vex::directionType::fwd, 12, vex::voltageUnits::volt);
  chassis.right_swing_to_angle(num + (reversed * 90));
  chassis.set_drive_exit_conditions(1.5, 300, 500);
  chassis.drive_distance(4, num + (reversed * 90), 6, 6);
  wait(1.5, vex::timeUnits::sec);
  Intake.stop(coast);
  chassis.set_drive_exit_conditions(1.5, 300, 400);
  chassis.drive_distance(-7, num + (reversed * 95));

  // Get the last ring
  Intake.spin(vex::directionType::fwd, 12, vex::voltageUnits::volt);
  chassis.right_swing_to_angle(0);
  wait(2, vex::timeUnits::sec);
  Intake.stop(coast);
  chassis.set_drive_exit_conditions(1.5, 300, 500);
  chassis.drive_distance(10, 0);

  // Touch the ladder
  chassis.turn_to_angle(num + (reversed * 270));
  Intake.spin(vex::directionType::fwd, 12, vex::voltageUnits::volt);
  chassis.set_drive_exit_conditions(1.5, 300, 2000);
  chassis.drive_distance(55, num + (reversed * 275));
}

// TESTED, BUT MOVED
void basic_negative(std::string c)
{
  int reversed;
  int num;
  if (c == "red")
  {
    reversed = -1;
    num = 360;
  }
  else if (c == "blue")
  {
    reversed = 1;
    num = 0;
  }
  odom_constants();
  chassis.set_heading(num + (reversed * 30));

  // Move backwards and clamp onto the goal
  chassis.drive_distance(-19, num + (reversed * 30));
  chassis.drive_distance(-5, num + (reversed * 30), 3, 3);
  Clamp.set(true);

  // Sweep the two rings on the edge of the line
  Intake.spin(vex::directionType::fwd, 12, vex::voltageUnits::volt);
  wait(500, vex::timeUnits::msec);
  chassis.turn_to_angle(num + (reversed * 145));
  chassis.drive_distance(19, num + (reversed * 135));
  chassis.right_swing_to_angle(num + (reversed * 90));
  chassis.drive_distance(4, num + (reversed * 90), 6, 6);
  wait(2, vex::timeUnits::sec);
  Intake.stop(coast);
  chassis.drive_distance(-7, num + (reversed * 95));

  // Get the last ring
  Intake.spin(vex::directionType::fwd, 12, vex::voltageUnits::volt);
  chassis.right_swing_to_angle(0);
  wait(2, vex::timeUnits::sec);
  Intake.stop(coast);
  chassis.drive_distance(10, 0);

  // Touch the ladder
  chassis.turn_to_angle(num + (reversed * 270));
  Intake.spin(vex::directionType::fwd, 12, vex::voltageUnits::volt);
  chassis.drive_distance(55, num + (reversed * 275));
}

// NOT DONE
void doinker_positive(std::string c)
{
  int reversed;
  int num;
  if (c == "red")
  {
    reversed = -1;
    num = 360;
  }
  else if (c == "blue")
  {
    reversed = 1;
    num = 0;
  }
  odom_constants();
}

// TESTED, BUT MOVED
void alliance_positive(std::string c)
{
  int reversed;
  int num;
  if (c == "red")
  {
    reversed = -1;
    num = 360;
  }
  else if (c == "blue")
  {
    reversed = 1;
    num = 0;
  }

  odom_constants();
  chassis.set_heading(num + (reversed * 270));

  // Get the Alliance Stake, pushing rings out of the way
  chassis.drive_distance(-9.5, num + (reversed * 270));
  chassis.turn_to_angle(180);
  chassis.drive_distance(-4, 180);
  Intake.spin(fwd, 12, volt);
  wait(800, msec);
  Intake.stop();

  // Move forwards
  chassis.drive_distance(13, 180);

  // Clamp onto the goal
  chassis.turn_to_angle(num + (reversed * 45));
  chassis.drive_distance(-30, num + (reversed * 45), 5, 2);
  wait(0.2, vex::timeUnits::sec);
  Clamp.set(true);
  wait(0.5, vex::timeUnits::sec);

  // Intake the ring on the bottom, closest to the mogo rush goal
  Intake.spin(vex::directionType::fwd, 12, vex::voltageUnits::volt);
  chassis.turn_to_angle(num + (reversed * 270));
  chassis.drive_distance(22);
  wait(2, vex::timeUnits::sec);
  Intake.stop(vex::brakeType::coast);

  // touch the ladder
  chassis.turn_to_angle(num + (reversed * 90));
  chassis.drive_distance(40, num + (reversed * 90), 8, 8);
}

// TESTED, BUT MOVED
void basic_positive(std::string c)
{
  int reversed;
  int num;
  if (c == "red")
  {
    reversed = -1;
    num = 360;
  }
  else if (c == "blue")
  {
    reversed = 1;
    num = 0;
  }
  // This autonomous gets 2 Rings, both on a Mogo
  odom_constants();
  chassis.set_heading(0);

  // Move backwards and clamp onto the goal
  // chassis.drive_distance(-15, 0);
  // chassis.turn_to_angle(330);
  // chassis.drive_distance(-5, 330, 6.5, 6.5, 0.1, 1000, 1000);
  chassis.drive_distance(-30, num + (reversed * 330), 5, 2);
  wait(0.2, vex::timeUnits::sec);
  Clamp.set(true);
  wait(0.5, vex::timeUnits::sec);

  // Intake the ring on the bottom, closest to the mogo rush goal
  Intake.spin(vex::directionType::fwd, 12, vex::voltageUnits::volt);
  chassis.turn_to_angle(num + (reversed * 270));
  chassis.drive_distance(22);
  wait(2, vex::timeUnits::sec);
  Intake.stop(vex::brakeType::coast);

  // touch the ladder
  chassis.turn_to_angle(num + (reversed * 90));
  chassis.drive_distance(40, num + (reversed * 90), 8, 8);
}

void alliance_negative_blue()
{
  alliance_negative("blue");
}

void basic_negative_blue()
{
  basic_negative("blue");
}

void doinker_positive_blue()
{
  doinker_positive("blue");
}

void alliance_positive_blue()
{
  alliance_positive("blue");
}

void basic_positive_blue()
{
  basic_positive("blue");
}

void alliance_negative_red()
{
  alliance_negative("red");
}

void basic_negative_red()
{
  basic_negative("red");
}

void doinker_positive_red()
{
  doinker_positive("red");
}

void alliance_positive_red()
{
  alliance_positive("red");
}

void basic_positive_red()
{
  basic_positive("red");
}

int intakeToFishyTaskWrapper()
{
  while (true)
  {
    intakeControl.intakeToFishyAutonTask();
    wait(100, vex::timeUnits::msec);
  }
  return 1;
}

int colorSortingTaskWrapper()
{
  while (true)
  {
    intakeControl.colorSortingAutonTask();
    wait(100, msec);
  }
  return 1;
}

int fishyAutonTaskWrapper()
{
  while (true)
  {
    fishyControl.fishyAutonTask();
    wait(20, vex::timeUnits::msec);
  }
  return 1;
}

// TODO: Not tested, done up until 2nd Wall Stake
void auton_skills()
{
  odom_constants();

  chassis.set_drive_exit_conditions(1.5, 300, 800);
  chassis.set_turn_exit_conditions(1, 300, 500);
  chassis.set_swing_exit_conditions(1, 300, 500);

  task intakeToFishyTask = task(intakeToFishyTaskWrapper);
  task colorSortingTask = task(colorSortingTask);
  task fishyAutonTask = task(fishyAutonTaskWrapper);

  intakeToFishyAuton = false;
  intakeSort = false;
  raiseFishy = false;

  chassis.set_heading(0);

  // Get the alliance stake
  Intake.spin(fwd, 12, volt);
  wait(500, msec);

  // Get the first mogo
  chassis.drive_distance(15, 0);
  chassis.turn_to_angle(270);
  chassis.drive_distance(-17, 270);
  chassis.drive_distance(-5, 270, 6, 6);
  Clamp.set(true);
  wait(500, vex::timeUnits::msec);

  // Get the first ring
  chassis.turn_to_angle(0);
  chassis.drive_distance(24, 0);

  // TODO: Not done yet, getting middle ring closest to wall stake and actually scoring on wall stake
  // get 2 rings, then score a wall stake
  Intake.spin(fwd, 12, volt);
  chassis.turn_to_angle(90);
  chassis.drive_distance(24, 90);
  chassis.turn_to_angle(15);
  chassis.drive_distance(27, 15);
  chassis.turn_to_angle(345);
  chassis.drive_distance(27, 345);
  wait(800, vex::timeUnits::msec);
  Intake.stop(brake);
  intakeToFishyAuton = true;
  chassis.turn_to_angle(0);
  chassis.drive_distance(-22, 0);
  chassis.turn_to_angle(90);
  chassis.drive_distance(14, 90);
  raiseFishy = true;
  wait(1000, msec);
  raiseFishy = false;

  // Collect the rest of the rings for 1st mogo
  chassis.drive_distance(-14, 90);
  chassis.turn_to_angle(180);
  chassis.set_drive_exit_conditions(1.5, 300, 3000);
  chassis.drive_distance(60, 180, 8, 8);
  wait(500, msec);
  chassis.turn_to_angle(45);
  chassis.set_drive_exit_conditions(1.5, 300, 800);
  chassis.drive_distance(17, 45);

  // Drop the 1st mogo in the corner
  chassis.drive_distance(-8.5, 45);
  chassis.turn_to_angle(315);
  chassis.drive_distance(-10, 315);
  Clamp.set(false);

  // Intake a ring, then bump the next one into the path
  chassis.set_drive_exit_conditions(1.5, 300, 3000);
  Intake.spin(fwd, 12, volt);
  chassis.drive_distance(73, 315);
  chassis.turn_to_angle(225);
  Intake.stop();
  chassis.set_drive_exit_conditions(1.5, 300, 800);
  chassis.drive_distance(34, 225, 12, 12);

  // Get the 2nd mogo
  chassis.turn_to_angle(0);
  chassis.drive_distance(19, 0);
  chassis.drive_distance(5, 0, 6, 6);
  Clamp.set(true);

  // Get 6 rings for the 2nd mogo
  Intake.spin(fwd, 12, volt);
  chassis.turn_to_angle(315);
  chassis.drive_distance(34, 315);
  chassis.turn_to_angle(180);
  chassis.set_drive_exit_conditions(1.5, 300, 3000);
  chassis.drive_distance(60, 180, 8, 8);
  wait(500, msec);
  chassis.turn_to_angle(315);
  chassis.set_drive_exit_conditions(1.5, 300, 800);
  chassis.drive_distance(17, 315);

  // Drop the 2nd mogo in the corner
  chassis.drive_distance(-8.5, 315);
  chassis.turn_to_angle(45);
  chassis.drive_distance(-10, 45);
  Intake.stop();
  Clamp.set(false);

  // Pickup a ring and score it on the Wall Stake
  chassis.drive_distance(5, 45);
  chassis.turn_to_angle(0);
  Intake.spin(fwd, 12, volt);
  chassis.drive_distance(50, 0);
  intakeToFishyAuton = true;
  chassis.turn_to_angle(270);
  chassis.drive_distance(5, 270);
  raiseFishy = true;
  wait(500, msec);
  raiseFishy = false;
}