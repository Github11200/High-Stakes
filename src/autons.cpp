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
  chassis.set_coordinates(120, 58, 0);
  Intake.spin(fwd, 12, volt);
  chassis.turn_to_angle(30);
  chassis.drive_distance(-35);
  Clamp.set(true);
  Intake.spin(fwd, 12, volt);
  Redirect.spinToPosition(150, deg, false);
  chassis.turn_to_point(117.33, 70.20);
  chassis.drive_distance(-35);
  Redirect.spinToPosition(0, deg, false);
  chassis.turn_to_point(117.33, 23.08);
  chassis.drive_to_point(117.33, 23.08);
  chassis.turn_to_point(93.77, 23.08);
  chassis.drive_to_point(93.77, 23.08);
  Redirect.spinToPosition(150, deg, false);
  chassis.turn_to_point(70.20, 70.20);
  chassis.drive_distance(-35);
}

void solo_awp_empty_side()
{
  odom_constants();
  chassis.set_coordinates(128.41, 58, 315);
  chassis.drive_distance(-10);
  Redirect.spinToPosition(150, deg, true);
  chassis.drive_distance(20);
  Redirect.spinToPosition(0, deg, false);
  chassis.drive_distance(-20);
  chassis.turn_to_angle(340);
  chassis.drive_distance(-35);
  Clamp.set(true);
  chassis.turn_to_point(93.77, 117.33);
  Intake.spin(fwd, 12, volt);
  chassis.drive_to_point(93.77, 117.33);
  Redirect.spinToPosition(150, deg, false);
  chassis.turn_to_point(117.33, 70.20);
  chassis.drive_to_point(117.33, 70.20);
  Redirect.spinToPosition(0, deg, false);
  chassis.drive_distance(-20);
  chassis.turn_to_angle(315);
  Intake.stop();
  chassis.drive_distance(15);
  chassis.turn_to_angle(270);
  chassis.drive_to_point(50, 128.41);
  chassis.drive_distance(-70);
  chassis.turn_to_point(70.20, 70.20);
  chassis.drive_distance(-35);
}

void three()
{
  odom_constants();
}

void four()
{
  odom_constants();
}

void five()
{
  odom_constants();
}