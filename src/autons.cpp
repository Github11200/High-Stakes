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

void one()
{
  odom_constants();
}

void two()
{
  odom_constants();
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