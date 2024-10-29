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

void three_ring_ladder_blue_goal_side()
{
  odom_constants();
  chassis.set_coordinates(115, 27, 0);

  // chassis.turn_to_angle(30);
  // chassis.drive_distance(-35);
  // Clamp.set(true);
  // //got the goal, scoring preload
  // Intake.spin(fwd, 12, volt);
  // Redirect.spinToPosition(150, deg, false);
  // chassis.turn_to_point(117.33, 70.20);
  // chassis.drive_distance(35);
  // Redirect.spinToPosition(0, deg, false);
  // //raised redirect to eat 2nd ring on stack
  // chassis.turn_to_point(117.33, 23.08);
  // chassis.drive_to_point(117.33, 23.08);
  // chassis.turn_to_point(93.77, 23.08);
  // chassis.drive_to_point(93.77, 23.08);
  // //ate 2 more rings
  // Redirect.spinToPosition(150, deg, false);
  // chassis.turn_to_point(70.20, 70.20);
  // chassis.drive_distance(-35);
  // //touched ladder
}

void solo_awp_empty_side()
{
  odom_constants();
  chassis.set_coordinates(128.41, 58, 315);
  chassis.drive_distance(-10);
  Redirect.spinToPosition(150, deg, true);
  chassis.drive_distance(20);
  Redirect.spinToPosition(0, deg, false);
  // scored a preload on the alliance stake
  chassis.drive_distance(-20);
  chassis.turn_to_angle(340);
  chassis.drive_distance(-35);
  Clamp.set(true);
  // got the goal
  chassis.turn_to_point(93.77, 117.33);
  Intake.spin(fwd, 12, volt);
  chassis.drive_to_point(93.77, 117.33);
  // ate a ring
  Redirect.spinToPosition(150, deg, false);
  chassis.turn_to_point(117.33, 70.20);
  chassis.drive_to_point(117.33, 70.20);
  Redirect.spinToPosition(0, deg, false);
  // raised redirect to eat 2nd ring on stack
  chassis.drive_distance(-20);
  chassis.turn_to_angle(315);
  Intake.stop();
  chassis.drive_distance(15);
  chassis.turn_to_angle(270);
  chassis.drive_to_point(50, 128.41);
  // maneuvering to the other side to push the other robot off the line
  chassis.drive_distance(-70);
  chassis.turn_to_point(70.20, 70.20);
  chassis.drive_distance(-35);
  // touched ladder
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
  Redirect.spinToPosition(150, deg, true);
  chassis.drive_distance(20);
  Redirect.spinToPosition(0, deg, false);
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