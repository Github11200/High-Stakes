#include "vex.h"
#include "../include/driver/intake.h"
#include "../include/driver/fishy.h"

using namespace vex;
using namespace std;

bool intakeToFishyAuton = false;
bool intakeSort = false;
bool raiseFishy = false;
bool fishing = false;
IntakeControl intakeControl(12, 3, OpticalSensor.hue());
Fishy fishyControl(12);

void default_constants()
{
  chassis.set_drive_constants(10, 0.7, 0, 2.5, 0);
  chassis.set_heading_constants(6, .4, 0, 0.1, 0);
  chassis.set_turn_constants(8, .3, 0.01, 3.5, 15);
  chassis.set_swing_constants(12, .3, .001, 2, 15);
  chassis.set_drive_exit_conditions(1.5, 300, 5000);
  chassis.set_turn_exit_conditions(1, 300, 1500);
  chassis.set_swing_exit_conditions(1, 300, 1000);
}

void odom_constants()
{
  default_constants();
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
  chassis.set_heading(0);

  // chassis.set_drive_exit_conditions(1.5, 300, 1000);
  // chassis.set_drive_constants(8, 1.2, 0, 10, 0);
  // chassis.set_turn_exit_conditions(1, 300, 500);
  // chassis.set_swing_exit_conditions(1, 300, 500);
  chassis.drive_to_point(0, 24);
  chassis.heading_max_voltage = 0;
  chassis.drive_to_point(24, 0);
  chassis.heading_max_voltage = 12;
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

// TODO: Done / Tested up until 2nd Wall Stake
void auton_skills()
{
  pre_driver = true;

  odom_constants();

  // chassis.drive_kd = 0.01;
  // chassis.turn_kd = 0.01;
  chassis.drive_max_voltage = 12;
  chassis.turn_max_voltage = 10;
  chassis.turn_timeout = 400;
  chassis.drive_timeout = 800;
  task colorSortingAutonTask = task(colorSortingAutonTaskWrapper);
  task intakeToFishyAutonTask = task(intakeToFishyAutonTaskWrapper);

  intakeSort = true;
  wait(0.5, vex::timeUnits::sec);
  intakeSort = false;

  // Get the mogo
  chassis.drive_to_point(-0.0288024, 13.5957);
  chassis.turn_to_point(21.5267, 13.4296, 180);
  chassis.drive_timeout = 1200;
  chassis.drive_to_point(21.5267, 13.4296, 5, 5);
  Clamp.set(true);
  wait(0.5, vex::timeUnits::sec);

  // Get the first ring
  intakeSort = true;
  chassis.drive_timeout = 800;
  chassis.turn_to_point(21.9998, 34.5713);
  chassis.drive_to_point(21.9998, 34.5713);

  // Get the second ring
  chassis.turn_to_point(46.8731, 36.399);
  chassis.drive_to_point(46.8731, 36.399);

  // Get the third ring
  chassis.turn_timeout = 400;
  chassis.turn_to_point(55.6561, 57.862);
  chassis.drive_to_point(55.6561, 57.862);

  // Get the fourth ring to fishy height
  chassis.turn_to_point(46.1794, 84.1463);
  chassis.drive_to_point(46.1794, 84.1463);

  // Move backwards to the wall stake
  intakeSort = false;
  intakeToFishyAuton = true;
  chassis.drive_timeout = 1000;
  chassis.turn_to_point(44.229, 61.5, 180);
  chassis.drive_to_point(44.229, 61.5);

  // Turn towards the wall stake and score on it
  chassis.turn_timeout = 800;
  chassis.turn_to_point(58.8379, 61.5);
  chassis.drive_to_point(58.8379, 61.5);
  chassis.turn_timeout = 400;
  chassis.turn_to_angle(90);
  fishyControl.liftFishy(true);

  intakeSort = true;
  intakeToFishyAuton = false;

  // Move backwards to align with the next two rings
  chassis.drive_timeout = 800;
  chassis.turn_to_point(41.4149, 59.4474, 180);
  chassis.drive_to_point(41.4149, 59.4474);

  // Get the first ring
  chassis.turn_to_point(43.1499, 16.7767);
  chassis.drive_to_point(43.1499, 16.7767);

  // Get both rings in the line
  chassis.drive_timeout = 800;
  chassis.turn_to_point(44.1072, 4.99533);
  chassis.drive_to_point(44.1072, 4.99533);
  wait(0.4, vex::timeUnits::sec);

  // Get the third ring in the triangle
  chassis.drive_timeout = 800;
  chassis.turn_timeout = 600;
  chassis.turn_to_point(54.1766, 10.8026);
  chassis.drive_to_point(54.1766, 10.8026);

  // Put the mogo in the corner
  chassis.turn_to_point(57.1338, 3.59061, 180);
  chassis.drive_to_point(57.1338, 3.59061);
  Clamp.set(false);

  // Go to the middle
  intakeSort = false;
  intakeToFishyAuton = true;
  chassis.drive_timeout = 2500;
  chassis.turn_to_point(-2.03885, 62.0337);
  chassis.drive_to_point(-2.03885, 62.0337);

  // Push the first ring into the position
  chassis.drive_timeout = 800;
  chassis.turn_timeout = 400;
  chassis.turn_to_point(-24.7068, 39.0372, 180);
  chassis.drive_to_point(-24.7068, 39.0372);
  wait(0.3, vex::timeUnits::sec);

  // Move forward again
  chassis.drive_timeout = 500;
  chassis.drive_to_point(-20.7873, 42.9426);

  // Get aligned for the mogo
  chassis.turn_to_point(-11.8323, 32.0713, 180);
  chassis.drive_to_point(-11.8323, 32.0713);

  // Turn and drive into the mogo to clamp onto it
  chassis.drive_timeout = 1200;
  chassis.turn_to_point(-21.6857, 15.8559, 180);
  chassis.drive_to_point(-21.6857, 15.8559, 5, 5);
  Clamp.set(1);
  wait(0.5, vex::timeUnits::sec);

  intakeSort = true;

  // Get the first ring
  chassis.drive_timeout = 800;
  chassis.turn_to_point(-33.9027, 29.3666);
  chassis.drive_to_point(-33.9027, 29.3666);

  // Get the second ring
  chassis.turn_to_point(-45.7288, 42.0187);
  chassis.drive_to_point(-45.7288, 42.0187);

  // Turn around and get the next two rings
  chassis.turn_timeout = 600;
  chassis.drive_timeout = 1500;
  chassis.turn_to_point(-46.1346, 20.2125);
  chassis.drive_to_point(-46.1346, 20.2125);

  // Get the second ring in the line
  chassis.drive_timeout = 800;
  chassis.turn_timeout = 500;
  chassis.turn_to_point(-46.2265, 8);
  chassis.drive_to_point(-46.2265, 8);

  // Get the last ring in the triangle
  chassis.turn_to_point(-55.2937, 16.3128);
  chassis.drive_to_point(-55.2937, 16.3128);

  // Put the mogo into the corner
  chassis.turn_to_point(-59.9038, 7.46229, 180);
  chassis.drive_to_point(-59.9038, 7.46229);
  Clamp.set(0);

  // Go to the wall stake, in front of it
  chassis.drive_timeout = 2000;
  chassis.turn_to_point(-43.4516, 61);
  chassis.drive_to_point(-43.4516, 61);

  intakeSort = false;
  intakeToFishyAuton = true;

  // Get the wall stake ring
  chassis.drive_timeout = 1500;
  chassis.turn_timeout = 800;
  chassis.turn_to_point(-54.2656, 60.5);
  chassis.drive_to_point(-54.2656, 60.5);
  chassis.turn_timeout = 600;

  // Score it onto the wall stake
  chassis.drive_timeout = 1000;
  chassis.drive_to_point(-59.8379, 60.5);
  chassis.turn_to_angle(270);
  fishyControl.liftFishy(true);

  intakeSort = false;
  intakeToFishyAuton = true;

  // Go backwards
  chassis.drive_timeout = 700;
  chassis.turn_timeout = 500;
  chassis.drive_to_point(-44.8193, 60.3206);

  // Get the first ring to fishy height
  chassis.turn_to_point(-44.6187, 86.7453);
  chassis.drive_to_point(-44.6187, 86.7453);

  // Go to the mogo
  chassis.drive_timeout = 800;
  chassis.turn_to_point(-26.2973, 106.497);
  chassis.drive_to_point(-26.2973, 106.497);

  // Get the mogo
  chassis.drive_timeout = 2000;
  chassis.turn_timeout = 800;
  chassis.turn_to_point(10.2748, 105.777, 180);
  chassis.drive_to_point(10.2748, 105.777, 5, 5);
  Clamp.set(1);
  wait(0.5, vex::timeUnits::sec);

  intakeSort = true;
  intakeToFishyAuton = false;

  // Get the second ring
  chassis.drive_timeout = 800;
  chassis.turn_timeout = 600;
  chassis.turn_to_point(24.1104, 80.4551);
  chassis.drive_to_point(24.1104, 80.4551);

  // Align to the corner
  chassis.turn_to_point(56.5896, 80.2526);
  chassis.drive_to_point(56.5896, 80.2526);

  // Push into the corner
  chassis.drive_timeout = 900;
  chassis.turn_to_point(64.122, 112.997, 180);
  chassis.drive_to_point(64.122, 112.997);
  Clamp.set(0);

  chassis.turn_to_point(43.6138, 97.5774);
  chassis.drive_to_point(43.6138, 97.5774);

  chassis.drive_timeout = 3000;
  chassis.turn_to_point(-48.3737, 122.738);
  chassis.drive_to_point(-48.3737, 122.738, 12, 12);

  chassis.drive_to_point(43.6138, 97.5774);
}