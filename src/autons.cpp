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
  chassis.set_turn_constants(12, .3, .03, 3, 15);
  chassis.set_swing_constants(12, .3, .001, 2, 15);
  chassis.set_drive_exit_conditions(1.5, 300, 5000);
  chassis.set_turn_exit_conditions(1, 300, 800);
  chassis.set_swing_exit_conditions(1, 300, 1000);
}

void odom_constants()
{
  default_constants();
  chassis.drive_max_voltage = 8;
  // chassis.drive_settle_error = 3;
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

int colorSortingAutonTaskWrapper()
{
  while (true)
  {
    intakeControl.colorSortingAutonTask();
    wait(50, vex::timeUnits::msec);
  }
  return 1;
}

int intakeToFishyAutonTaskWrapper()
{
  while (true)
  {
    intakeControl.intakeToFishyAutonTask();
    wait(50, vex::timeUnits::msec);
  }
  return 1;
}

// TESTED, BUT MOVED FROM ALLIANCE_NEGATIVE_BLUE TO ALLIANCE_NEGATIVE
void alliance_negative(std::string c)
{
  task fishyAutonTask = task(fishyAutonTaskWrapper);
  task colorSortingAutonTask = task(colorSortingAutonTaskWrapper);
  task intakeToFishyAutonTask = task(intakeToFishyAutonTaskWrapper);
  int reversed;
  int num;
  if (c == "blue")
  {
    reversed = -1;
    num = 360;
  }
  else if (c == "red")
  {
    reversed = 1;
    num = 0;
  }
  odom_constants();
  chassis.set_heading(num + (reversed * 90));

  wait(1, vex::timeUnits::sec);

  // Get the Alliance Stake, pushing rings out of the way
  chassis.set_drive_exit_conditions(1.5, 300, 900);
  chassis.drive_distance(-12.5, num + (reversed * 90));
  chassis.set_turn_exit_conditions(1, 300, 400);
  chassis.turn_to_angle(180);
  chassis.set_drive_exit_conditions(1.5, 300, 600);
  chassis.drive_distance(-7, 180, 4, 8);
  intakeSort = true;
  wait(600, msec);
  intakeSort = false;

  // Move forwards
  chassis.set_drive_exit_conditions(1.5, 300, 900);
  chassis.drive_distance(4.3, 180, 6, 6);

  // Move backwards and clamp onto the goal
  chassis.set_turn_exit_conditions(1, 300, 700);
  chassis.turn_to_angle(num + (reversed * 322));
  chassis.set_drive_exit_conditions(1.5, 300, 1000);
  chassis.drive_distance(-31, num + (reversed * 322), 7, 12);
  chassis.drive_distance(-5, num + (reversed * 322), 3, 3);
  Clamp.set(true);
  wait(200, msec);

  // Sweep the two rings on the edge of the line
  chassis.turn_to_angle(num + (reversed * 145));
  intakeSort = true;
  chassis.set_drive_exit_conditions(1.5, 300, 800);
  chassis.drive_distance(19.5, num + (reversed * 145));
  if (c == "red")
    chassis.right_swing_to_angle(num + (reversed * 90));
  else
    chassis.left_swing_to_angle(num + (reversed * 90));
  chassis.set_drive_exit_conditions(1.5, 300, 700);
  chassis.drive_distance(8, num + (reversed * 90), 6, 6);
  wait(1.8, vex::timeUnits::sec);
  intakeSort = false;
  chassis.set_drive_exit_conditions(1.5, 300, 600);
  chassis.drive_distance(-11, num + (reversed * 95));

  // Get the last ring
  intakeSort = true;
  if (c == "red")
    chassis.right_swing_to_angle(0);
  else
    chassis.left_swing_to_angle(0);
  if (c == "red")
    chassis.right_swing_to_angle(num + (reversed * 270));
  else
    chassis.left_swing_to_angle(num + (reversed * 270));

  // chassis.set_drive_exit_conditions(1.5, 300, 2000);
  // chassis.drive_distance(55, num + (reversed * 275));
}

// TESTED, BUT MOVED
void basic_negative(std::string c)
{
  int reversed;
  int num;
  if (c == "blue")
  {
    reversed = -1;
    num = 360;
  }
  else if (c == "red")
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

  chassis.set_heading(270);

  // chassis.set_drive_exit_conditions(1.5, 300, 1000);
  // chassis.set_drive_constants(8, 1.2, 0, 10, 0);
  // chassis.set_turn_exit_conditions(1, 300, 500);
  // chassis.set_swing_exit_conditions(1, 300, 500);

  // chassis.drive_distance(-19, 270);
  // chassis.drive_distance(-5, 270, 6, 6);

  FishyMech.spinFor(30, vex::rotationUnits::deg, false);
}

// TESTED, BUT MOVED
void alliance_positive(std::string c)
{
  int reversed;
  int num;
  if (c == "blue")
  {
    reversed = -1;
    num = 360;
  }
  else if (c == "red")
  {
    reversed = 1;
    num = 0;
  }
  odom_constants();
  chassis.set_heading(num + (reversed * 270));

  // Get the Alliance Stake, pushing rings out of the way
  chassis.set_drive_exit_conditions(1.5, 300, 900);
  chassis.drive_distance(-12.5, num + (reversed * 270));
  chassis.set_turn_exit_conditions(1, 300, 400);
  chassis.turn_to_angle(180);
  chassis.set_drive_exit_conditions(1.5, 300, 600);
  chassis.drive_distance(-7, 180, 4, 8);
  Intake.spin(fwd, 12, volt);
  wait(800, msec);
  Intake.stop();

  chassis.set_drive_exit_conditions(1.5, 300, 900);
  chassis.drive_distance(4.3, 180, 6, 6);

  // Move backwards and clamp onto the goal
  chassis.set_turn_exit_conditions(1, 300, 700);
  chassis.turn_to_angle(num + (reversed * 40));
  chassis.set_drive_exit_conditions(1.5, 300, 1000);
  chassis.drive_distance(-32, num + (reversed * 40), 7, 12);
  chassis.drive_distance(-5, num + (reversed * 40), 3, 3);
  Clamp.set(true);
  wait(200, msec);

  // Intake the ring on the bottom, closest to the mogo rush goal
  Intake.spin(vex::directionType::fwd, 12, vex::voltageUnits::volt);
  chassis.turn_to_angle(num + (reversed * 270));
  chassis.drive_distance(22);
  wait(2, vex::timeUnits::sec);
  Intake.stop(vex::brakeType::coast);

  // touch the ladder
  chassis.turn_to_angle(num + (reversed * 90));
  FishyMech.spinFor(30, vex::rotationUnits::deg, false);
  chassis.drive_distance(40, num + (reversed * 90), 8, 8);
}

// TESTED, BUT MOVED
void basic_positive(std::string c)
{
  int reversed;
  int num;
  if (c == "blue")
  {
    reversed = -1;
    num = 360;
  }
  else if (c == "red")
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
  chassis.drive_distance(-30, num + (reversed * 30), 5, 2);
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
  FishyMech.spinFor(30, vex::rotationUnits::deg, false);
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

// TODO: Not tested, done up until 2nd Wall Stake
void auton_skills()
{
  odom_constants();
  chassis.set_heading_constants(8, 0.6, 0, 1, 0);
  task fishyAutonTask = task(fishyAutonTaskWrapper);
  task colorSortingAutonTask = task(colorSortingAutonTaskWrapper);
  task intakeToFishyAutonTask = task(intakeToFishyAutonTaskWrapper);

  intakeSort = true;
  wait(0.5, vex::timeUnits::sec);

  // Go to the mogo
  chassis.drive_to_point(0, 12.2574);
  chassis.turn_to_angle(270);
  chassis.drive_to_point(17.0812, 12.0596, 5, 5);
  Clamp.set(true);
  wait(0.5, vex::timeUnits::sec);

  // Get first ring
  chassis.turn_to_angle(14.9412);
  chassis.drive_to_point(22.8474, 33.9692);

  // Get the second ring
  chassis.turn_to_angle(81.4123);
  chassis.drive_to_point(41.9384, 36.7118);

  // Get the third ring
  chassis.turn_to_angle(33.9591);
  chassis.drive_to_point(55.3286, 56.4116);

  // Get fourth ring for fishy
  intakeSort = false;
  chassis.turn_to_angle(341.638);
  intakeToFishyAuton = true;
  chassis.drive_to_point(48.0972, 78.4638);

  // Go to wall stake and score
  chassis.turn_to_angle(0);
  chassis.drive_to_point(47.7277, 59.9461);
  chassis.turn_to_angle(85.9013);
  chassis.drive_to_point(55.3324, 60.395);
  intakeToFishyAuton = false;
  intakeSort = false;
  fishyControl.liftFishy(true); // Score the ring on the wall stake

  // Get another ring in the corner
  intakeSort = true;
  wait(0.5, vex::timeUnits::sec); // Wait for the fishy to be lowered
  chassis.drive_to_point(40.3485, 59.7954);
  chassis.turn_to_angle(175.451);
  chassis.drive_to_point(45.3911, 5.70993);

  // Backup and get another ring
  chassis.drive_to_point(43.3524, 26.3621);
  chassis.turn_to_angle(135.904);
  chassis.drive_to_point(55.5296, 13.88);

  // Put the mogo in the corner
  chassis.turn_to_angle(341.532);
  chassis.drive_to_point(58.3944, 6.07385);
  Clamp.set(false);
  wait(0.5, vex::timeUnits::sec);

  // Move bot forward and turn for next point, and get the ring under the ladder, and get it to fishy height
  intakeSort = false;
  chassis.drive_to_point(54.3845, 18.259);
  chassis.turn_to_angle(305.804);
  chassis.drive_to_point(-6.03039, 58.858);
  intakeToFishyAuton = true;

  // chassis.set_drive_exit_conditions(1.5, 300, 1000);
  // chassis.set_drive_constants(8, 1.2, 0, 10, 0);
  // chassis.set_turn_exit_conditions(1, 300, 500);
  // chassis.set_swing_exit_conditions(1, 300, 500);

  // task fishyAutonTask = task(fishyAutonTaskWrapper);
  // task colorSortingAutonTask = task(colorSortingAutonTaskWrapper);
  // task intakeToFishyAutonTask = task(intakeToFishyAutonTaskWrapper);

  // intakeToFishyAuton = false;
  // intakeSort = false;
  // raiseFishy = false;
  // pre_driver = true;

  // chassis.set_heading(0);

  // // Get the alliance stake
  // intakeSort = true;
  // wait(500, msec);
  // intakeSort = false;

  // // Get the first mogo
  // chassis.drive_distance(12, 0, 6, 6);
  // chassis.turn_to_angle(270);
  // chassis.drive_distance(-16, 270);
  // chassis.drive_distance(-5, 270, 6, 6);
  // Clamp.set(true);
  // wait(500, vex::timeUnits::msec);

  // // Get the first ring
  // intakeSort = true;
  // chassis.turn_to_angle(0);
  // chassis.drive_distance(24, 0);

  // // TODO: Not done yet, getting middle ring closest to wall stake and actually scoring on wall stake
  // // get 2 rings, then score a wall stake
  // chassis.turn_to_angle(90);
  // chassis.drive_distance(24, 90);
  // chassis.turn_to_angle(15);
  // chassis.drive_distance(27, 15);
  // chassis.turn_to_angle(345);
  // chassis.drive_distance(27, 345);
  // intakeSort = false;
  // intakeToFishyAuton = true;
  // chassis.turn_to_angle(0);
  // chassis.drive_distance(-24, 0);
  // chassis.turn_to_angle(90);
  // chassis.drive_distance(14, 90);
  // raiseFishy = true;
  // wait(1000, msec);

  // // Collect the rest of the rings for 1st mogo
  // chassis.drive_distance(-11, 90);
  // raiseFishy = false;
  // chassis.turn_to_angle(180);
  // intakeSort = true;
  // chassis.set_drive_exit_conditions(1.5, 300, 3000);
  // chassis.drive_distance(53, 180, 8, 8);
  // wait(500, msec);
  // chassis.turn_to_angle(45);
  // chassis.set_drive_exit_conditions(1.5, 300, 800);
  // chassis.drive_distance(17, 45);

  // // Drop the 1st mogo in the corner
  // chassis.drive_distance(-8.5, 45);
  // chassis.turn_to_angle(315);
  // chassis.drive_distance(-10, 315);
  // intakeSort = false;
  // Clamp.set(false);
  // wait(1000, vex::timeUnits::msec);

  // // Intake a ring, then bump the next one into the path
  // chassis.set_drive_exit_conditions(1.5, 300, 3000);
  // intakeSort = true;
  // chassis.drive_distance(79, 315);
  // chassis.turn_to_angle(225);
  // intakeSort = false;
  // chassis.set_drive_exit_conditions(1.5, 300, 800);
  // chassis.drive_distance(34, 225, 12, 12);

  // // Get the 2nd mogo
  // chassis.turn_to_angle(0);
  // chassis.drive_distance(-19, 0);
  // chassis.drive_distance(-5, 0, 6, 6);
  // Clamp.set(true);

  // // Get 6 rings for the 2nd mogo
  // intakeSort = true;
  // chassis.turn_to_angle(315);
  // chassis.drive_distance(34, 315);
  // chassis.turn_to_angle(180);
  // chassis.set_drive_exit_conditions(1.5, 300, 3000);
  // chassis.drive_distance(60, 180, 8, 8);
  // wait(500, msec);
  // chassis.turn_to_angle(315);
  // chassis.set_drive_exit_conditions(1.5, 300, 800);
  // chassis.drive_distance(17, 315);

  // // Drop the 2nd mogo in the corner
  // chassis.drive_distance(-8.5, 315);
  // chassis.turn_to_angle(45);
  // chassis.drive_distance(-10, 45);
  // intakeSort = false;
  // Clamp.set(false);

  // // Pickup a ring and score it on the Wall Stake
  // chassis.drive_distance(5, 45);
  // chassis.turn_to_angle(0);
  // intakeToFishyAuton = true;
  // chassis.drive_distance(50, 0);
  // chassis.turn_to_angle(270);
  // chassis.drive_distance(5, 270);
  // raiseFishy = true;
  // wait(500, msec);
  // raiseFishy = false;

  // // Get a ring, then grab the 3rd Mogo
  // chassis.drive_distance(-14, 270);
  // chassis.turn_to_angle(45);
  // intakeSort = true;
  // chassis.drive_distance(34, 45);
  // intakeSort = false;
  // chassis.turn_to_angle(225);
  // chassis.drive_distance(-29, 225);
  // chassis.drive_distance(-5, 225);
  // Clamp.set(true);

  // // Get some more rings
  // intakeSort = true;
}