#ifndef DRIVE_H
#define DRIVE_H

#include "vex.h"

enum drive_setup
{
  ZERO_TRACKER_NO_ODOM,
  ZERO_TRACKER_ODOM,
  TANK_ONE_FORWARD_ENCODER,
  TANK_ONE_FORWARD_ROTATION,
  TANK_ONE_SIDEWAYS_ENCODER,
  TANK_ONE_SIDEWAYS_ROTATION,
  TANK_TWO_ENCODER,
  TANK_TWO_ROTATION,
  HOLONOMIC_TWO_ENCODER,
  HOLONOMIC_TWO_ROTATION
};

struct DriveParams
{
  float drive_min_voltage = 0;
  float drive_max_voltage = 12;
  float heading_max_voltage = 12;
  float drive_settle_error = 0.2;
  float drive_settle_time = 500;
  float drive_timeout = 4000;
  float drive_kp = 0.413;
  float drive_ki = 0.1;
  float drive_kd = 0.5;
  float drive_starti = 0;
  float drive_slew = 0.8;
  float heading_kp = 0.06;
  float heading_ki = 0;
  float heading_kd = 0;
  float heading_starti = 0;

  DriveParams &set_min_voltage(float val)
  {
    drive_min_voltage = val;
    return *this;
  }

  DriveParams &set_max_voltage(float val)
  {
    drive_max_voltage = val;
    return *this;
  }

  DriveParams &set_heading_max_voltage(float val)
  {
    heading_max_voltage = val;
    return *this;
  }

  DriveParams &set_settle_error(float val)
  {
    drive_settle_error = val;
    return *this;
  }

  DriveParams &set_settle_time(float val)
  {
    drive_settle_time = val;
    return *this;
  }

  DriveParams &set_timeout(float val)
  {
    drive_timeout = val;
    return *this;
  }

  DriveParams &set_kp(float val)
  {
    drive_kp = val;
    return *this;
  }

  DriveParams &set_ki(float val)
  {
    drive_ki = val;
    return *this;
  }

  DriveParams &set_kd(float val)
  {
    drive_kd = val;
    return *this;
  }

  DriveParams &set_starti(float val)
  {
    drive_starti = val;
    return *this;
  }

  DriveParams &set_drive_slew(float val)
  {
    drive_slew = val;
    return *this;
  }

  DriveParams &set_heading_kp(float val)
  {
    heading_kp = val;
    return *this;
  }

  DriveParams &set_heading_ki(float val)
  {
    heading_ki = val;
    return *this;
  }

  DriveParams &set_heading_kd(float val)
  {
    heading_kd = val;
    return *this;
  }

  DriveParams &set_heading_starti(float val)
  {
    heading_starti = val;
    return *this;
  }
};

struct TurnParams
{
  float turn_max_voltage = 10;
  float turn_settle_error = 1;
  float turn_settle_time = 500;
  float turn_timeout = 2000;
  float turn_kp = 0.19;
  float turn_ki = 0;
  float turn_kd = 1.3;
  float turn_starti = 0;

  TurnParams &set_max_voltage(float val)
  {
    turn_max_voltage = val;
    return *this;
  }

  TurnParams &set_settle_error(float val)
  {
    turn_settle_error = val;
    return *this;
  }

  TurnParams &set_settle_time(float val)
  {
    turn_settle_time = val;
    return *this;
  }

  TurnParams &set_timeout(float val)
  {
    turn_timeout = val;
    return *this;
  }

  TurnParams &set_kp(float val)
  {
    turn_kp = val;
    return *this;
  }

  TurnParams &set_ki(float val)
  {
    turn_ki = val;
    return *this;
  }

  TurnParams &set_kd(float val)
  {
    turn_kd = val;
    return *this;
  }

  TurnParams &set_starti(float val)
  {
    turn_starti = val;
    return *this;
  }
};

struct SwingParams
{
  float swing_max_voltage = 12;
  float swing_settle_error = 1;
  float swing_settle_time = 5000;
  float swing_timeout = 5000;
  float swing_kp = 0.3;
  float swing_ki = 0.01;
  float swing_kd = 2;
  float swing_starti = 3;

  SwingParams &set_max_voltage(float val)
  {
    swing_max_voltage = val;
    return *this;
  }

  SwingParams &set_settle_error(float val)
  {
    swing_settle_error = val;
    return *this;
  }

  SwingParams &set_settle_time(float val)
  {
    swing_settle_time = val;
    return *this;
  }

  SwingParams &set_timeout(float val)
  {
    swing_timeout = val;
    return *this;
  }

  SwingParams &set_kp(float val)
  {
    swing_kp = val;
    return *this;
  }

  SwingParams &set_ki(float val)
  {
    swing_ki = val;
    return *this;
  }

  SwingParams &set_kd(float val)
  {
    swing_kd = val;
    return *this;
  }

  SwingParams &set_starti(float val)
  {
    swing_starti = val;
    return *this;
  }
};

/**
 * Drive class supporting tank and holo drive, with or without odom.
 * Eight flavors of odom and six custom motion algorithms.
 */

class Drive
{
private:
  float wheel_diameter;
  float wheel_ratio;
  float gyro_scale;
  float drive_in_to_deg_ratio;
  float ForwardTracker_center_distance;
  float ForwardTracker_diameter;
  float ForwardTracker_in_to_deg_ratio;
  float SidewaysTracker_center_distance;
  float SidewaysTracker_diameter;
  float SidewaysTracker_in_to_deg_ratio;
  vex::triport ThreeWire = vex::triport(vex::PORT22);

public:
  drive_setup drive_setup = ZERO_TRACKER_NO_ODOM;
  motor_group DriveL;
  motor_group DriveR;
  inertial Gyro;
  motor DriveLF;
  motor DriveRF;
  motor DriveLB;
  motor DriveRB;
  rotation R_ForwardTracker;
  rotation R_SidewaysTracker;
  encoder E_ForwardTracker;
  encoder E_SidewaysTracker;
  double imu_constant;

  float turn_max_voltage;
  float turn_kp;
  float turn_ki;
  float turn_kd;
  float turn_starti;

  float turn_settle_error;
  float turn_settle_time;
  float turn_timeout;

  float drive_min_voltage;
  float drive_max_voltage;
  float drive_kp;
  float drive_ki;
  float drive_kd;
  float drive_starti;

  float drive_settle_error;
  float drive_settle_time;
  float drive_timeout;

  float heading_max_voltage;
  float heading_kp;
  float heading_ki;
  float heading_kd;
  float heading_starti;

  float swing_max_voltage;
  float swing_kp;
  float swing_ki;
  float swing_kd;
  float swing_starti;

  float swing_settle_error;
  float swing_settle_time;
  float swing_timeout;

  float boomerang_lead;
  float boomerang_setback;

  Drive(enum ::drive_setup drive_setup, motor_group DriveL, motor_group DriveR, int gyro_port, float wheel_diameter, float wheel_ratio, float gyro_scale, int DriveLF_port, int DriveRF_port, int DriveLB_port, int DriveRB_port, int ForwardTracker_port, float ForwardTracker_diameter, float ForwardTracker_center_distance, int SidewaysTracker_port, float SidewaysTracker_diameter, float SidewaysTracker_center_distance);

  void drive_with_voltage(float leftVoltage, float rightVoltage);

  float get_absolute_heading();
  float get_left_position_in();
  float get_right_position_in();

  void set_turn_constants(float turn_max_voltage, float turn_kp, float turn_ki, float turn_kd, float turn_starti);
  void set_drive_constants(float drive_max_voltage, float drive_kp, float drive_ki, float drive_kd, float drive_starti);
  void set_heading_constants(float heading_max_voltage, float heading_kp, float heading_ki, float heading_kd, float heading_starti);
  void set_swing_constants(float swing_max_voltage, float swing_kp, float swing_ki, float swing_kd, float swing_starti);

  void set_turn_exit_conditions(float turn_settle_error, float turn_settle_time, float turn_timeout);
  void set_drive_exit_conditions(float drive_settle_error, float drive_settle_time, float drive_timeout);
  void set_swing_exit_conditions(float swing_settle_error, float swing_settle_time, float swing_timeout);

  void turn_to_angle(float angle, TurnParams turnParams = {});
  void drive_distance(float distance, float heading, DriveParams driveParams = {});
  void left_swing_to_angle(float angle, SwingParams swingParams = {});
  void right_swing_to_angle(float angle, SwingParams swingParams = {});
  void drive_to_point(float X_position, float Y_position, DriveParams driveParams = {});
  void drive_to_pose(float X_position, float Y_position, float angle, float lead, float setback, DriveParams driveParams = {});
  void turn_to_point(float X_position, float Y_position, float extra_angle_deg, TurnParams turnParams = {});
  void holonomic_drive_to_pose(float X_position, float Y_position, float angle, DriveParams driveParams = {}, TurnParams turnParams = {});

  Odom odom;
  float get_ForwardTracker_position();
  float get_SidewaysTracker_position();
  void set_coordinates(float X_position, float Y_position, float orientation_deg);
  void calibrate_robot();
  void set_heading(float orientation_deg);
  void position_track();
  static int position_track_task();
  void stop_position_track_task();
  vex::task odom_task;
  float get_X_position();
  float get_Y_position();

  void drive_stop(vex::brakeType mode);

  void control_arcade();
  void control_tank();
  void control_holonomic();
};

#endif