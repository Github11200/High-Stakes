#include "vex.h"

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

void redirect_pls()
{
  // Intake the ring until it reaches the limit switch
  while (RedirectLimitSwitch.pressing())
    Intake.spin(vex::directionType::fwd, 11, vex::voltageUnits::volt);
  Intake.setVelocity(100, pct);
  Intake.spinFor(fwd, 500, msec);
  if (RedirectLimitSwitch.pressing())
  {
    redirect_pls();
    return;
  }
  while (!RedirectLimitSwitch.pressing())
    Intake.spin(vex::directionType::fwd, 6, vex::voltageUnits::volt);

  Intake.spinFor(reverse, 1000, msec);
  Intake.stop();
}

void test()
{
  chassis.set_coordinates(0, 0, 0);
  chassis.drive_distance(-3);

  // chassis.turn_to_point(24, 24);
  chassis.drive_to_point(5, 100);
  //  chassis.drive_to_point(0,0);
  //  chassis.turn_to_angle(180);
}

// DONE
void three_ring_ladder_blue_goal_side()
{
  odom_constants();
  chassis.set_coordinates(115, 39.25, 0);

  // Move backwards and clamp onto the goal
  chassis.drive_distance(-19.01);
  Clamp.set(true);

  // Intake the ring on the bottom, closest to the mogo rush goal
  Intake.spin(vex::directionType::fwd, 12, vex::voltageUnits::volt);
  chassis.turn_to_angle(114);
  chassis.drive_distance(23);
  wait(1, vex::timeUnits::sec);
  Intake.stop(vex::brakeType::coast);

  // Turn around and intake the other ring that is on top of the stack closest to the ladder
  chassis.turn_to_angle(297);
  Intake.spin(vex::directionType::fwd, 12, vex::voltageUnits::volt);
  chassis.drive_distance(47);
  wait(1, vex::timeUnits::sec);
  Intake.stop(vex::brakeType::coast);

  // Turn around and touch the ladder
  chassis.turn_to_angle(172);
  chassis.drive_distance(13);
}

// DONE
void solo_awp_empty_side()
{
  odom_constants();
  chassis.set_coordinates(115, 86.25, 0);

  // Move backwards and clamp onto the goal
  chassis.drive_distance(-19.01);
  Clamp.set(true);

  // Turn around and intake the ring on the bottom of the stack
  Intake.spin(vex::directionType::fwd, 12, vex::voltageUnits::volt);
  chassis.turn_to_angle(246);
  chassis.drive_distance(23);
  wait(1, vex::timeUnits::sec);

  // Intake the two rings that are on the bottom of the stacks one by one
  // We move forward, intake, move back again, turn a bit to face the second ring, and repeat the same thing
  chassis.turn_to_angle(178);
  chassis.drive_distance(17);
  wait(1, vex::timeUnits::sec);
  chassis.drive_distance(-17);
  chassis.turn_to_angle(271);
  wait(1, vex::timeUnits::sec);
  chassis.drive_distance(17);
  chassis.drive_distance(-17);
  Intake.stop(vex::brakeType::coast);

  // Turn to face the ladder and touch it
  chassis.turn_to_angle(196);
  chassis.drive_distance(23);
}

void three()
{
  odom_constants();
}

void four()
{
  odom_constants();
}

void auton_skills()
{
  odom_constants();
  chassis.set_coordinates(15, 70.20, 0);
  chassis.turn_to_point(0, 70.20);
  FishyMech.spinToPosition(150, deg, true);
  chassis.drive_distance(20);
  FishyMech.spinToPosition(0, deg, false);
  // scored a preload on the alliance stake
  chassis.turn_to_point(46.64, 93.77);
  Intake.spin(fwd, 12, volt);
  chassis.drive_distance(33);
  Intake.stop();
  chassis.drive_to_point(23.08, 70.20);
  chassis.turn_to_angle(90);
  chassis.drive_distance(-20);
  Clamp.set(true);
  // got bottom left mogo
  Intake.spin(fwd, 12, volt);
  chassis.turn_to_point(46.64, 46.64);
  chassis.drive_to_point(46.64, 46.64);
  chassis.turn_to_point(46.64, 23.08);
  chassis.drive_to_point(46.64, 23.08);
  chassis.turn_to_point(11.30, 23.08);
  chassis.drive_to_point(11.30, 23.08);
  chassis.turn_to_point(23.08, 11.30);
  chassis.drive_to_point(23.08, 11.30);
  // cleaned up all bottom left rings
  chassis.turn_to_angle(30);
  chassis.drive_distance(-15);
  Intake.stop();
  Clamp.set(false);
  // dropped goal in bottom left corner
  Intake.spin(fwd, 12, volt);
  chassis.turn_to_point(70.20, 70.20);
  chassis.drive_to_point(70.20, 70.20);
  chassis.turn_to_angle(333);
  Intake.stop();
  chassis.drive_distance(50);
  Clamp.set(true);
  // got upper left mogo
  Intake.spin(fwd, 12, volt);
  chassis.turn_to_point(46.64, 117.33);
  chassis.drive_to_point(46.64, 117.33);
  chassis.turn_to_point(11.30, 117.33);
  chassis.drive_to_point(11.30, 117.33);
  chassis.turn_to_point(23.08, 129);
  chassis.drive_to_point(23.08, 129);
  // cleaned up all upper left rings
  chassis.turn_to_angle(335);
  chassis.drive_distance(-15);
  Intake.stop();
  Clamp.set(false);
  // dropped goal in upper left corner
}