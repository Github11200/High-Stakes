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
  chassis.set_drive_constants(10, 0.9, 0.05, 2, 0);
  chassis.set_heading_constants(6, .4, 0, 0.1, 0);
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

  task colorSortingAutonTask = task(colorSortingAutonTaskWrapper);
  task intakeToFishyAutonTask = task(intakeToFishyAutonTaskWrapper);

  intakeSort = true;
  wait(0.5, vex::timeUnits::sec);
  intakeSort = false;

  // Get the mogo
  chassis.drive_to_point(-0.0288024, 13.5957);
  intakeSort = true;
  chassis.turn_to_point(21.5267, 13.4296, 180);
  chassis.drive_to_point(21.5267, 13.4296, 5, 5);
  Clamp.set(true);
  wait(0.5, vex::timeUnits::sec);

  // Get the first ring
  chassis.turn_to_point(21.9998, 34.5713);
  chassis.drive_to_point(21.9998, 34.5713);

  // Get the second ring
  chassis.turn_to_point(46.8731, 36.399);
  chassis.drive_to_point(46.8731, 36.399);

  // Get the third ring
  chassis.turn_to_point(55.6561, 57.862);
  chassis.drive_to_point(55.6561, 57.862);

  // Get the fourth ring to fishy height
  chassis.turn_to_point(46.1794, 84.1463);
  chassis.drive_to_point(46.1794, 84.1463);

  // Move backwards to the wall stake
  intakeSort = false;
  intakeToFishyAuton = true;
  chassis.turn_to_point(44.229, 59.3248, 180);
  chassis.drive_to_point(44.229, 59.3248);

  // Turn towards the wall stake and score on it
  chassis.turn_to_point(57.4387, 60.614);
  chassis.drive_to_point(57.4387, 60.614);
  chassis.turn_to_angle(89);
  fishyControl.liftFishy(true);

  intakeSort = true;
  intakeToFishyAuton = false;

  // Move backwards to align with the next two rings
  chassis.turn_to_point(41.4149, 59.4474, 180);
  chassis.drive_to_point(41.4149, 59.4474);

  // Get the first ring
  chassis.turn_to_point(43.1499, 20.7767);
  chassis.drive_to_point(43.1499, 20.7767);

  // Get the second ring
  chassis.turn_to_point(44.1072, 4.99533);
  chassis.drive_to_point(44.1072, 4.99533);

  // Get the third ring in the triangle
  chassis.turn_to_point(54.1766, 10.8026);
  chassis.drive_to_point(54.1766, 10.8026);

  // Put the mogo in the corner
  chassis.turn_to_point(57.1338, 3.59061, 180);
  chassis.drive_to_point(57.1338, 3.59061);
  Clamp.set(false);

  // Go to the middle
  intakeSort = false;
  intakeToFishyAuton = true;
  chassis.turn_to_point(-2.03885, 62.0337);
  chassis.drive_to_point(-2.03885, 62.0337);

  // Push the ring into alignment
  chassis.turn_to_point(-31.4976, 34.4265, 180);
  chassis.drive_to_point(-31.4976, 34.4265, 7, 7);

  // Move forward
  chassis.drive_to_point(-20.0331, 44.9562);

  // Move backwards a bit
  chassis.turn_to_point(-13.7708, 30.5816, 180);
  chassis.drive_to_point(-13.7708, 30.5816);

  // Move backwards to clamp onto the mogo
  chassis.turn_to_point(-25.1667, 14.2865, 180);
  chassis.drive_to_point(-25.1667, 14.2865, 5, 5);
  Clamp.set(true);
  wait(0.5, vex::timeUnits::sec);

  intakeSort = true;
  intakeToFishyAuton = false;

  // Get the first ring we pushed into alignment
  chassis.turn_to_point(-35.3955, 24.7023);
  chassis.drive_to_point(-35.3955, 24.7023);
  chassis.turn_to_point(-46.37, 38.8311);

  // Get the second ring
  chassis.drive_to_point(-46.37, 38.8311);
  chassis.turn_to_point(-48.0636, 19.1817);

  // Turn around for the first ring the line of 2
  chassis.turn_to_point(-48.9689, 6.622);
  chassis.drive_to_point(-48.9689, 6.622);

  // Get the second one in the line
  chassis.turn_to_point(-58.0796, 14.7376);
  chassis.drive_to_point(-58.0796, 14.7376);

  // Put the mogo in the corner
  chassis.turn_to_point(-62.3841, 5.64864);
  chassis.drive_to_point(-62.3841, 5.64864);
  Clamp.set(false);
  wait(0.5, vex::timeUnits::sec);

  // Go to the ladder for second wall stake
  chassis.turn_to_point(-38.8515, 60.4602);
  chassis.drive_to_point(-38.8515, 60.4602);

  // Get the first ring to fishy height
  intakeSort = false;
  intakeToFishyAuton = true;
  chassis.turn_to_point(-55.2288, 61.7321);
  chassis.drive_to_point(-55.2288, 61.7321);

  // Score it on the wall stake
  chassis.turn_to_point(-60.64, 62.0037);
  chassis.drive_to_point(-60.64, 62.0037);
  fishyControl.liftFishy();

  //////////////////////////////////////////////////////////////////////
  //////////////////////////////////////////////////////////////////////
  //////////////////////////////////////////////////////////////////////

  return;

  // Move forward, turn, move back, and then clamp
  chassis.drive_timeout = 800;
  chassis.drive_to_point(-0.0310672, 12.4142);
  chassis.turn_timeout = 600;
  chassis.turn_to_angle(269.231);
  chassis.drive_timeout = 1200;
  chassis.drive_to_point(20.8049, 12.6687, 5, 5);
  Clamp.set(true);
  wait(0.3, vex::timeUnits::sec);
  intakeSort = true;

  // Intake the first ring
  chassis.turn_to_angle(3.36246);
  chassis.drive_timeout = 900;
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

  // Score on the first wall stake
  chassis.turn_to_angle(0.568702);
  chassis.drive_timeout = 1200;
  chassis.drive_to_point(46.5324, 62.5);
  chassis.turn_timeout = 1000;
  chassis.turn_to_angle(90, 6);
  chassis.turn_timeout = 600;
  chassis.heading_max_voltage = 0;
  chassis.drive_to_point(56, 62.5);
  chassis.heading_max_voltage = 12;
  chassis.turn_to_angle(90);
  fishyControl.liftFishy(true);
  chassis.drive_timeout = 800;

  // Drive backwards, and then get two rings
  intakeToFishyAuton = false;
  intakeSort = true;
  chassis.drive_to_point(45, 61.4521);
  chassis.turn_to_angle(180);
  chassis.drive_timeout = 1200;
  chassis.drive_to_point(45, 19.2499);
  wait(0.8, vex::timeUnits::sec);
  chassis.drive_timeout = 900;
  chassis.heading_max_voltage = 0;
  chassis.drive_to_point(45, 3);
  wait(0.6, vex::timeUnits::sec);
  chassis.heading_max_voltage = 12;
  chassis.turn_to_point(51.3819, 8.3219);

  // Get the last ring and then put the mogo in the corner
  chassis.drive_to_point(51.3819, 8.3219);
  wait(0.3, vex::timeUnits::sec);
  chassis.turn_to_angle(328.392);
  chassis.drive_timeout = 1200;
  chassis.drive_to_point(60, 2);
  Clamp.set(false);
  wait(0.2, vex::timeUnits::sec);
  intakeSort = false;
  chassis.drive_timeout = 900;
  chassis.drive_to_point(52.7827, 8.95865);
  chassis.turn_to_angle(313.013);

  // Get the ring in the middle of the ladder up to fishy height
  chassis.drive_timeout = 2000;
  intakeToFishyAuton = true;
  chassis.drive_to_point(-3.3988, 60);
  wait(0.5, vex::timeUnits::sec);
  chassis.turn_to_angle(47.7477);

  // Move the first ring a bit
  chassis.drive_timeout = 900;
  chassis.drive_to_point(-31.3932, 36.6471);
  chassis.drive_to_point(-27.1846, 40.1795);

  // // Get the mogo
  chassis.drive_timeout = 1000;
  chassis.turn_to_angle(319.586);
  chassis.drive_to_point(-17.4068, 28.9699);
  chassis.turn_to_angle(31.9539);
  chassis.drive_timeout = 1200;
  chassis.drive_to_point(-28.0793, 12.1399, 5, 5);
  Clamp.set(true);
  wait(0.2, vex::timeUnits::sec);
  intakeToFishyAuton = false;
  intakeSort = true;

  // // Get the ring we pushed and the one in front of it
  chassis.turn_to_angle(320.777);
  chassis.heading_max_voltage = 0;
  chassis.drive_timeout = 800;
  chassis.drive_to_point(-37.9108, 24.3597);
  wait(0.5, vex::timeUnits::sec);
  chassis.drive_to_point(-49, 36.4124);
  chassis.heading_max_voltage = 12;

  // // Turn around and get the next two rings
  chassis.turn_to_angle(180);
  chassis.drive_to_point(-52, 20.1324);
  wait(0.8, vex::timeUnits::sec);
  chassis.drive_to_point(-52, 6, 12, 12);

  // // Get the last ring in the corner
  wait(0.8, vex::timeUnits::sec);
  chassis.turn_to_angle(300.262);
  chassis.drive_to_point(-58.8051, 10.9887);
  wait(0.5, vex::timeUnits::sec);

  // // Put the mogo into the corner
  chassis.turn_to_point(-63.5111, 2.88708);
  chassis.drive_to_point(-63.5111, 2.88708);
  Clamp.set(false);
  wait(0.5, vex::timeUnits::sec);

  // // Move the bot to the position for scoring on the wall stake
  chassis.drive_timeout = 1200;
  chassis.drive_to_point(-59.7098, 10.6803);
  chassis.drive_timeout = 3000;
  chassis.drive_to_point(-47.1768, 59);
  chassis.turn_to_angle(270);
  intakeSort = false;
  intakeToFishyAuton = true;
  chassis.heading_max_voltage = 0;
  chassis.drive_timeout = 1000;
  chassis.drive_to_point(-58, 59);
  wait(0.4, vex::timeUnits::sec);
  chassis.turn_to_angle(270);
  chassis.drive_to_point(-66, 59);
  chassis.heading_max_voltage = 12;
  chassis.turn_to_angle(270);
  fishyControl.liftFishy(true);

  intakeSort = false;
  intakeToFishyAuton = true;

  chassis.drive_to_point(-39.2448, 59.5934);
  chassis.turn_to_angle(36.5733);

  intakeSort = true;
  intakeToFishyAuton = false;

  chassis.drive_to_point(-20.4187, 83.5587);
  chassis.turn_to_angle(225.896);
  chassis.drive_to_point(7.26771, 109.253);
  chassis.turn_to_angle(150.074);
  chassis.drive_to_point(24.2518, 79.8849);
  chassis.turn_to_angle(91.8114);
  chassis.drive_to_point(47.318, 79.149);
  chassis.turn_to_angle(3.87799);
  chassis.drive_to_point(48.8148, 97.6529);
  chassis.drive_to_point(47.8471, 86.3584);
  chassis.turn_to_angle(38.6868);
  chassis.drive_to_point(60.1454, 101.647);
  chassis.drive_to_point(52.8495, 92.5354);
  chassis.turn_to_angle(276.436);
  chassis.drive_to_point(-24.2425, 99.4669);
  chassis.turn_to_angle(245.916);
  chassis.drive_to_point(-49.6559, 87.9983);
  chassis.turn_to_angle(345.15);
  chassis.drive_to_point(-55.1902, 109.692);
  chassis.turn_to_angle(158.116);
  chassis.drive_to_point(-60.0302, 125.62);
  chassis.drive_to_point(-55.2008, 104.904);
  chassis.turn_to_angle(266.764);
  chassis.drive_to_point(-0.763797, 111.795);
  chassis.turn_to_angle(185.268);
  chassis.drive_to_point(0.642623, 126.413);
  chassis.drive_to_point(0.247733, 122.736);
  chassis.turn_to_angle(272.855);
  chassis.drive_to_point(55.153, 121.7);

  // // chassis.drive_to_point(-57.5014, 59.3201);
  // // chassis.turn_to_angle(54.5915);
  // // chassis.drive_to_point(-26.9954, 83.3325);
  // // chassis.turn_to_angle(230.27);
  // // chassis.drive_to_point(-4.18818, 101.507);
  // // chassis.turn_to_angle(135.086);
  // // chassis.drive_to_point(17.7783, 79.4236);
  // // chassis.turn_to_angle(54.0095);
  // // chassis.drive_to_point(39.3642, 95.1756);
  // // chassis.turn_to_angle(22.0353);
  // // chassis.turn_to_angle(19.8004);
  // // chassis.drive_to_point(42.3674, 102.287);
  // // chassis.drive_to_point(38.7883, 92.4839);
  // // chassis.drive_to_point(38.7899, 92.4882);
  // // chassis.turn_to_angle(70.126);
  // // chassis.drive_to_point(48.7116, 96.4242);
  // // chassis.turn_to_angle(270.212);
  // // chassis.drive_to_point(-40.4402, 94.5496);
  // // chassis.turn_to_angle(222.471);
  // // chassis.drive_to_point(-50.4491, 83.3744);
  // // chassis.turn_to_angle(221.511);
  // // chassis.turn_to_angle(308.182);
  // // chassis.drive_to_point(-61.0608, 91.635);
  // // chassis.turn_to_angle(3.11556);
  // // chassis.drive_to_point(-60.9121, 97.7107);
  // // chassis.turn_to_angle(173.032);
  // // chassis.drive_to_point(-62.992, 115.034);
  // // chassis.drive_to_point(-62.9916, 115.022);
  // // chassis.drive_to_point(-61.4036, 88.183);
  // // chassis.turn_to_angle(70.3755);
  // // chassis.drive_to_point(0.1321, 113.81);
}