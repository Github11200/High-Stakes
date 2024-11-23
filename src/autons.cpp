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
  pre_driver = true;

  odom_constants();
  chassis.drive_kd = 0.01;
  chassis.turn_kd = 0.01;
  chassis.drive_max_voltage = 7;
  chassis.turn_max_voltage = 10;

  task colorSortingAutonTask = task(colorSortingAutonTaskWrapper);
  task intakeToFishyAutonTask = task(intakeToFishyAutonTaskWrapper);

  intakeSort = true;
  wait(0.5, vex::timeUnits::sec);

  // Move forward, turn, move back, and then clamp
  chassis.drive_to_point(-0.0310672, 12.4142);
  chassis.turn_to_angle(269.231);
  chassis.drive_to_point(20.8049, 12.6687, 5, 5);
  Clamp.set(true);
  wait(0.5, vex::timeUnits::sec);

  // Intake the first ring
  chassis.turn_to_angle(3.36246);
  chassis.drive_to_point(21.9351, 33.6442);

  // Intake the second ring to the right
  chassis.turn_to_angle(80.6756);
  chassis.drive_to_point(41.4478, 36.9391);

  // Intake the ring that is close to the wall stake
  chassis.turn_to_angle(33.4638);
  chassis.drive_to_point(54.4096, 56.6027);

  // Intake the ring after the wall stake
  chassis.turn_to_angle(343.97);
  wait(0.5, vex::timeUnits::sec);
  intakeSort = false;
  intakeToFishyAuton = true;
  chassis.drive_to_point(46.9024, 82.0763);
  wait(0.5, vex::timeUnits::sec);

  // Score on the wall stake
  chassis.turn_to_angle(0.568702);
  chassis.drive_to_point(46.5324, 61.3799);
  chassis.turn_to_angle(88);
  chassis.turn_max_voltage = 0;
  chassis.drive_to_point(55, 64);
  fishyControl.liftFishy(true);
  chassis.turn_max_voltage = 10;

  // // Drive backwards, and then get two rings 
  intakeToFishyAuton = false;
  intakeSort = true;
  chassis.drive_to_point(41.6902, 61.4521);
  chassis.turn_to_angle(180.282); 
  chassis.drive_to_point(41.9364, 19.2499);
  chassis.drive_to_point(41.9765, 6.52784);
  chassis.turn_to_angle(60.199);

  chassis.drive_to_point(51.3819, 11.3219);
  chassis.turn_to_angle(328.392);
  chassis.drive_to_point(55.5463, 4.48858);
  Clamp.set(false);
  wait(0.2, vex::timeUnits::sec);
  chassis.drive_to_point(52.7827, 8.95865);
  chassis.turn_to_angle(313.013);

  intakeSort = false;
  intakeToFishyAuton = true;
  chassis.drive_to_point(-3.3988, 57.4869);
  chassis.turn_to_angle(47.7477); 

  intakeToFishyAuton = false;
  intakeSort = true;
  chassis.drive_to_point(-31.3932, 36.6471);
  chassis.drive_to_point(-27.1846, 40.1795);

  // Get the mogo
  chassis.turn_to_angle(358.237);
  chassis.drive_to_point(-26.2572, 11.1215, 5, 5);
  Clamp.set(true);

  chassis.turn_to_angle(326.634);
  chassis.drive_to_point(-35.9648, 26.0121);
  chassis.drive_to_point(-45.2021, 39.9434);
  chassis.turn_to_angle(185.824);
  chassis.drive_to_point(-47.2771, 20.7892);
  chassis.drive_to_point(-48.3828, 10.8891);
  chassis.drive_to_point(-45.8931, 31.2277);
  chassis.turn_to_angle(222.901);
  chassis.drive_to_point(-57.3782, 18.8355);

  // Put the mogo into the corner
  chassis.turn_to_angle(27.0028);
  chassis.drive_to_point(-63.4052, 7.18981);
  Clamp.set(false);
  wait(0.5, vex::timeUnits::sec);
}