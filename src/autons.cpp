#include "vex.h"

using namespace vex;
using namespace std;

bool fishyTime = false;

int intakeTask()
{
  while (true)
  {
    // Intake.spin(fwd, 12, volt);
    // if (OpticalSensor.color() != alliance)
    // {
    //   Intake.setVelocity(100, pct);
    //   Intake.spinFor(fwd, 2000, msec);
    //   Intake.stop(brake);
    //   vex::wait(400, msec);
    // }
    vex::wait(100, msec);
  }

  Intake.stop();
  return 0;
}

int intakeToFishyTask()
{
  while (true)
  {
    if (fishyTime)
    {
      // Spin the intake until the optical sensor senses a ring color
      while (!OpticalSensor.isNearObject())
        Intake.spin(vex::directionType::fwd, 12, vex::voltageUnits::volt);
      Intake.stop(vex::brakeType::brake);
      fishyTime = false;
    }
  }
  return 1;
}

void scoreFishy()
{
  while (FishyMech.position(degrees) < 240)
  {
    Intake.spin(vex::directionType::fwd, 6, vex::voltageUnits::volt);
    FishyMech.spin(vex::directionType::fwd, 12, vex::voltageUnits::volt);
  }
  while (FishyMech.position(degrees) > 3)
  {
    Intake.spin(vex::directionType::rev, 6, vex::voltageUnits::volt);
    FishyMech.spin(vex::directionType::rev, (FishyMech.position(degrees) / 240) * 12 + 3, vex::voltageUnits::volt);
  }
}

void default_constants()
{
  chassis.set_drive_constants(10, 1.5, 0, 10, 0);
  chassis.set_heading_constants(6, .4, 0, 1, 0);
  chassis.set_turn_constants(12, .4, .03, 3, 15);
  chassis.set_swing_constants(12, .3, .001, 2, 15);
  chassis.set_drive_exit_conditions(1.5, 300, 5000);
  chassis.set_turn_exit_conditions(1, 300, 3000);
  chassis.set_swing_exit_conditions(1, 300, 3000);
}

void odom_constants()
{
  default_constants();
  chassis.drive_max_voltage = 8;
  chassis.drive_settle_error = 3;
}

// NOT TESTED
void alliance_negative_blue()
{
  odom_constants();

  // Get the alliance stake, pushing the rings out of the way
  chassis.set_heading(270);
  chassis.drive_distance(-12, 270);
  chassis.turn_to_angle(180);
  chassis.drive_distance(-10, 180);
  Intake.spin(fwd, 12, volt);
  wait(0.5, vex::timeUnits::sec);
  Intake.stop(coast);
  chassis.drive_distance(14, 180);

  // Move backwards and clamp onto the goal
  chassis.turn_to_angle(315);
  chassis.drive_distance(-34, 315);
  chassis.drive_distance(-5, 315, 6.5, 6.5, 0.1, 1000, 1000);
  wait(0.1, vex::timeUnits::sec);
  Clamp.set(true);
  wait(0.5, vex::timeUnits::sec);

  // Sweep the two rings on the edge of the line
  Intake.spin(vex::directionType::fwd, 12, vex::voltageUnits::volt);
  chassis.turn_to_angle(225);
  chassis.drive_distance(8, 225);
  chassis.left_swing_to_angle(270);
  chassis.drive_distance(18, 270, 6.5, 6.5);
  chassis.drive_distance(-8, 225);

  // Get the last ring
  chassis.left_swing_to_angle(0);
  chassis.drive_distance(12, 0);

  // Touch the ladder
  chassis.turn_to_angle(90);
  chassis.drive_distance(55, 90);
}

// NOT TESTED
void basic_negative_blue()
{
  odom_constants();
  chassis.set_heading(330);

  // Move backwards and clamp onto the goal
  chassis.drive_distance(-23, 330);
  chassis.drive_distance(-5, 330, 6.5, 6.5, 0.1, 1000, 1000);
  wait(0.1, vex::timeUnits::sec);
  Clamp.set(true);
  wait(0.5, vex::timeUnits::sec);

  // Sweep the two rings on the edge of the line
  Intake.spin(vex::directionType::fwd, 12, vex::voltageUnits::volt);
  chassis.turn_to_angle(225);
  chassis.drive_distance(8, 225);
  chassis.left_swing_to_angle(270);
  chassis.drive_distance(18, 270, 6.5, 6.5);
  chassis.drive_distance(-8, 225);

  // Get the last ring
  chassis.left_swing_to_angle(0);
  chassis.drive_distance(12, 0);

  // Touch the ladder
  chassis.turn_to_angle(90);
  chassis.drive_distance(55, 90);
  // wait(0.2, vex::timeUnits::sec);
  // chassis.turn_to_angle(270);
  // chassis.drive_distance(23);
  // wait(0.4, vex::timeUnits::sec);

  // // Intake the two rings that are on the bottom of the stacks one by one
  // // We move forward, intake, move back again, turn a bit to face the second ring, and repeat the same thing
  // chassis.turn_to_angle(170);
  // chassis.drive_distance(14.5, 170, 3, 3);
  // wait(0.3, vex::timeUnits::sec);
  // chassis.drive_distance(-14.5);
  // chassis.turn_to_angle(193);
  // wait(0.3, vex::timeUnits::sec);
  // chassis.drive_distance(15.5, 193, 4, 4);
  // wait(0.5, vex::timeUnits::sec);
  // chassis.drive_distance(-15);

  // Turn to face the ladder and touch it
  chassis.turn_to_angle(90);
  chassis.drive_distance(40);
  Intake.stop(coast);
}

// NOT DONE
void doinker_positive_blue()
{
  odom_constants();
}

// NOT TESTED
void alliance_positive_blue()
{
  // This autonomous gets 2 Rings, 1 on the Alliance Stake and 1 on a Mogo

  odom_constants();
  chassis.set_heading(90);

  // Get the Alliance Stake, pushing rings out of the way
  chassis.drive_distance(-12, 90);
  chassis.turn_to_angle(180);
  chassis.drive_distance(-10, 180);
  Intake.spin(fwd, 12, volt);
  wait(500, msec);
  Intake.stop();

  // Move backwards and clamp onto the goal
  chassis.turn_to_angle(0);
  chassis.drive_distance(-14, 0);
  chassis.turn_to_angle(45);
  chassis.drive_distance(-29, 45);
  chassis.drive_distance(-5, 45, 6.5, 6.5, 0.1, 1000, 1000);
  wait(0.2, vex::timeUnits::sec);
  Clamp.set(true);
  wait(0.5, vex::timeUnits::sec);

  // Intake the ring on the bottom, closest to the mogo rush goal
  Intake.spin(vex::directionType::fwd, 12, vex::voltageUnits::volt);
  chassis.turn_to_angle(90);
  chassis.drive_distance(22);
  wait(2, vex::timeUnits::sec);
  Intake.stop(vex::brakeType::coast);

  //touch the ladder
  chassis.turn_to_angle(270);
  chassis.drive_distance(35);
}

// NOT TESTED
void basic_positive_blue()
{
  // This autonomous gets 2 Rings, both on a Mogo

  odom_constants();
  chassis.set_heading(0);

  // Move backwards and clamp onto the goal
  chassis.drive_distance(-23, 0);
  chassis.turn_to_angle(30);
  chassis.drive_distance(-5, 30, 6.5, 6.5, 0.1, 1000, 1000);
  wait(0.2, vex::timeUnits::sec);
  Clamp.set(true);
  wait(0.5, vex::timeUnits::sec);

  // Intake the ring on the bottom, closest to the mogo rush goal
  Intake.spin(vex::directionType::fwd, 12, vex::voltageUnits::volt);
  chassis.turn_to_angle(90);
  chassis.drive_distance(22);
  wait(2, vex::timeUnits::sec);
  Intake.stop(vex::brakeType::coast);

  //touch the ladder
  chassis.turn_to_angle(270);
  chassis.drive_distance(35);
}

// WILL MIRROR FROM BLUE ONCE BLUE IS TESTED
void alliance_negative_red()
{
  odom_constants();
}

// WILL MIRROR FROM BLUE ONCE BLUE IS TESTED
void basic_negative_red()
{
  odom_constants();
  chassis.set_heading(210);

  // Move backwards and clamp onto the goal
  chassis.drive_distance(-23, 210);
  chassis.drive_distance(-5, 210, 6.5, 6.5, 0.1, 1000, 1000);
  // chassis.drive_distance(-27, 30, 6.5, 6.5);
  wait(0.1, vex::timeUnits::sec);
  Clamp.set(true);
  wait(0.5, vex::timeUnits::sec);

  // Turn around and intake the ring on the bottom of the stack
  Intake.spin(vex::directionType::fwd, 12, vex::voltageUnits::volt);
  wait(0.2, vex::timeUnits::sec);
  chassis.turn_to_angle(270);
  chassis.drive_distance(23);
  wait(0.4, vex::timeUnits::sec);

  // Intake the two rings that are on the bottom of the stacks one by one
  // We move forward, intake, move back again, turn a bit to face the second ring, and repeat the same thing
  chassis.turn_to_angle(343);
  chassis.drive_distance(15, 343, 3, 3);
  wait(0.3, vex::timeUnits::sec);
  chassis.drive_distance(-15);
  chassis.turn_to_angle(10); // PREVIOUSLY 15
  wait(0.3, vex::timeUnits::sec);
  chassis.drive_distance(16.5, 10, 4, 4);
  wait(0.5, vex::timeUnits::sec);
  chassis.drive_distance(-16);

  // Turn to face the ladder and touch it
  chassis.turn_to_angle(90);
  chassis.drive_distance(40);
  Intake.stop(coast);
}

// WILL MIRROR FROM BLUE ONCE BLUE IS TESTED
void doinker_positive_red()
{
  odom_constants();
}

// WILL MIRROR FROM BLUE ONCE BLUE IS TESTED
void alliance_positive_red()
{
  odom_constants();
}

// WILL MIRROR FROM BLUE ONCE BLUE IS TESTED
void basic_positive_red()
{
  odom_constants();
  chassis.set_heading(180);

  // Move backwards and clamp onto the goal
  chassis.drive_distance(-23, 180);
  chassis.drive_distance(-5, 180, 6.5, 6.5, 0.1, 1000, 1000);
  wait(0.2, vex::timeUnits::sec);
  Clamp.set(true);
  wait(0.5, vex::timeUnits::sec);

  // Intake the ring on the bottom, closest to the mogo rush goal
  Intake.spin(vex::directionType::fwd, 12, vex::voltageUnits::volt);
  chassis.turn_to_angle(85);
  chassis.drive_distance(22);
  wait(2, vex::timeUnits::sec);
  Intake.stop(vex::brakeType::coast);

  //touch the ladder
  chassis.turn_to_angle(270);
  chassis.drive_distance(35);
}

void auton_skills()
{
  odom_constants();
  fishyTime = false;
  chassis.set_heading(0);

  // Get the alliance stake
  Intake.spin(fwd, 12, volt);
  wait(500, msec);

  // Get the first mogo
  chassis.drive_distance(15, 0);
  chassis.turn_to_angle(270);
  chassis.drive_distance(-22, 270, 6, 6);
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
  fishyTime = true;
  chassis.turn_to_angle(0);
  chassis.drive_distance(-22, 0);
  chassis.turn_to_angle(90);
  chassis.drive_distance(14, 90);

  scoreFishy();
}