#include "vex.h"
#include "../include/driver/intake.h"
#include "../include/driver/fishy.h"

using namespace vex;
using namespace std;

bool intakeToFishyAuton = false;
bool intakeSort = false;
bool intakeRev = false;
bool raiseFishy = false;
bool fishing = false;
IntakeControl intakeControl(12, 3, OpticalSensor.hue());
Fishy fishyControl(12);

void default_constants()
{
  chassis.set_drive_constants(12, 10, 0, 3, 0, 20);
  chassis.set_heading_constants(12, 2, 0, 10, 0, 0);
  chassis.set_turn_constants(12, 2, 0, 10, 15);
  chassis.set_swing_constants(12, 2, 0, 10, 15);
  chassis.set_drive_exit_conditions(1.5, 50, 5000);
  chassis.set_turn_exit_conditions(1, 50, 1500);
  chassis.set_swing_exit_conditions(1, 50, 1000);
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

// DONE
void negative_alliance_stake_rush(string c)
{
  pre_driver = true;

  task colorSortingAutonTask = task(colorSortingAutonTaskWrapper);
  int reversed;
  if (c == "blue")
  {
    reversed = -1;
    alliance = "blue"; 
  }
  else if (c == "red")
  {
    reversed = 1;
    alliance = "red";
  }
  odom_constants();

  // Move in front of the wall stake
  chassis.turn_to_point(-0.1998 * reversed, -11.3, 180);
  chassis.drive_to_point(-0.1998 * reversed, -11.3); 

  // Move back and score
  chassis.turn_to_point(-6.0 * reversed, -11.8, 180);
  chassis.drive_to_point(-6.0 * reversed, -11.3);
 
  intakeSort = true;
  wait(0.5, vex::timeUnits::sec);

  // Move forward a bit
  chassis.turn_to_point(0.693042 * reversed, -11.3);
  chassis.drive_to_point(0.693042 * reversed, -11.3);
 
  intakeSort = false;

  // Go to the mogo and clamp
  chassis.turn_to_point(37.0626 * reversed, 18.1247, 180);
  chassis.drive_to_point(37.0626 * reversed, 18.1247, 6, 6); 
  Clamp.set(1);
  wait(0.5, vex::timeUnits::sec);
 
  intakeSort = true;

  chassis.turn_to_point(-39.7294, 21.122);
  chassis.drive_to_point(-39.7294, 21.122);

  chassis.drive_timeout = 350;
  chassis.drive_kp = 1.1;
  chassis.drive_kd = 2.7;

  chassis.drive_to_point(-41.9998*reversed, 25.8261);
  chassis.drive_to_point(-42.7085*reversed, 32.1277);
  chassis.drive_to_point(-42.5917*reversed, 24.83);
  chassis.drive_to_point(-39.8135*reversed, 30.8944);

  chassis.turn_to_point(-35.3707, 0.239032);
  chassis.drive_to_point(-35.3707, 0.239032, 7, 7);

}

// DONE
void positive_alliance_stake_rush(string c)
{
  pre_driver = true;

  task colorSortingAutonTask = task(colorSortingAutonTaskWrapper);

  int reversed;
  if (c == "blue")
  {
    reversed = -1;
  }
  else if (c == "red")
  {
    reversed = 1;
  }
  odom_constants();

  // Move backwards to align with the alliance stake horizontally
  chassis.turn_to_point(0 * reversed, -10.9, 180);
  chassis.drive_to_point(0 * reversed, -10.9);

  // Move backwards to the alliance stake
  chassis.turn_to_point(5.78 * reversed, -10.8, 180);
  chassis.drive_to_point(5.7 * reversed, -10.8);

  intakeSort = true;
  wait(0.5, vex::timeUnits::sec);

  // Move forward a bit
  chassis.turn_to_point(0, -10.8);
  chassis.drive_to_point(0, -10.8);

  intakeSort = false;
 
  // Move to the mogo and clamp it
  chassis.turn_to_point(-31.441 * reversed, 15.7054, 180);
  chassis.drive_to_point(-31.441 * reversed, 15.7054, 6, 6);
  Clamp.set(1);
  wait(0.5, vex::timeUnits::sec);
 
  intakeSort = true;

  // Get the ring
  chassis.turn_to_point(-30.2483 * reversed, 31.1189);
  chassis.drive_to_point(-30.2483 * reversed, 31.1189);
  wait(0.5, vex::timeUnits::sec);

  // Drive to the ladder
  chassis.turn_to_point(-30.2422 * reversed, -2);
  chassis.drive_to_point(-30.2422 * reversed, -2, 7, 7);
}

// DONE
void negative_ring_rush(string c)
{
  pre_driver = true;

  task colorSortingAutonTask = task(colorSortingAutonTaskWrapper);
  int reversed;
  if (c == "red")
  {
    reversed = -1;
    alliance = "red";
  }
  else if (c == "blue")
  {
    reversed = 1;
    alliance = "blue";
  }
  odom_constants();

  // Get the mogo
  chassis.drive_timeout = 550;
  chassis.drive_to_point(-8 * reversed, -26, 8, 1.8);
  Clamp.set(true);
  intakeSort = true;
  chassis.drive_to_point(-8 * reversed, -26, 8, 1.8);

  // Turn to face the two rings
  chassis.turn_to_point(-11.3642 * reversed, -28.155);

  chassis.turn_max_voltage = 8;
  chassis.heading_max_voltage = 8;
  chassis.drive_max_voltage = 11;
  chassis.drive_kp = 1;
  chassis.drive_timeout = 250;

  // Get the two rings in the 8 ring stack and then the 1 ring under, it should end facing the corner
  chassis.drive_to_point(-16.1906 * reversed, -33.0502 + 1);
  chassis.drive_to_point(-20.4246 * reversed, -37.6321 + 1);
  chassis.drive_to_point(-23.8813 * reversed, -40.3071 + 1);
  chassis.drive_to_point(-12.3734 * reversed, -29.1557 + 1);
  chassis.drive_to_point(-27.9062 * reversed, -41.9403 + 1);
  chassis.drive_to_point(-32.3782 * reversed, -42.0858 + 1);
  chassis.drive_to_point(-26.4813 * reversed, -42.2028 + 1);
  chassis.drive_to_point(-21.3751 * reversed, -42.2713 + 1);
  chassis.drive_to_point(-26.4054 * reversed, -39.7361 + 1);
  chassis.drive_to_point(-28.918 * reversed, -34.6516 + 1);

  chassis.drive_timeout = 1000;

  // Go around the ring
  chassis.drive_to_point(-27.3593 * reversed, -31.4806 + 1);
  chassis.drive_to_point(-23.3215 * reversed, -22.7789 + 1);
  chassis.drive_to_point(-25.2396 * reversed, -14.2957 + 1);
  chassis.drive_to_point(-30.9209 * reversed, -8.08665 + 1);

  chassis.drive_timeout = 700;

  // Turn on the doinker and go to the corner
  Doinker.set(true);
  chassis.turn_to_point(-42.2461 * reversed, 2.94447);
  chassis.drive_to_point(-42.2461 * reversed, 2.94447);

  chassis.turn_timeout = 900;

  // Clear the corner
  chassis.turn_timeout = 2000;
  chassis.turn_to_angle(alliance == "blue" ? 150 : 53, 4);
  Doinker.set(false);
  chassis.turn_to_angle(alliance == "blue" ? 46.8294 : 313.17, 12);
  chassis.turn_timeout = 400;

  chassis.drive_timeout = 500;

  // Turn back to the corner and intake
  chassis.turn_to_point(-44.7168 * reversed, 5.41391);
  chassis.drive_to_point(-44.7168 * reversed, 5.41391);

  chassis.drive_timeout = 1000;

  // Move back from the corner
  chassis.turn_to_point(-30.9613 * reversed, -18.9326);
  chassis.drive_to_point(-30.9613 * reversed, -18.9326);

  chassis.drive_timeout = 3000;

  // Touch the ladder
  chassis.turn_to_point(2.01298 * reversed, -28.6099);
  chassis.drive_to_point(2.01298 * reversed, -28.6099);
}

// NOT DONE
void positive_corner_rush(string c)
{
  pre_driver = true;

  task colorSortingAutonTask = task(colorSortingAutonTaskWrapper);
  int reversed;
  if (c == "red")
  {
    reversed = -1;
    alliance = "red";
  }
  else if (c == "blue")
  {
    reversed = 1;
    alliance = "blue";
  }
  odom_constants();

  // Get the mogo
  chassis.drive_timeout = 550;
  chassis.turn_timeout = 250;
  chassis.drive_to_point(-8 * reversed, -26, 8, 1.8);
  Clamp.set(true);
  intakeSort = true;
  chassis.drive_to_point(-8 * reversed, -26, 8, 1.8);

  // Turn and get a ring
  chassis.drive_timeout = 800;
  chassis.turn_timeout = 400;
  chassis.turn_to_point(8.68866 * reversed, -25.3646 - 1.5);
  chassis.drive_to_point(8.68866 * reversed, -25.3646 - 1.5);
  vex::wait(700, msec);

  chassis.drive_timeout = 400;
  chassis.turn_timeout = 400;
  // Push the blue ring forwards, reposition
  chassis.turn_to_point(15.1719 * reversed, -25.0983 - 1.5);
  intakeSort = false;
  intakeRev = true;
  chassis.drive_to_point(15.1719 * reversed, -25.0983 - 1.5);

  // Push the other blue ring away, then come back
  chassis.drive_timeout = 900;
  chassis.turn_to_point(15 * reversed, 4.25326 - 1.5);
  chassis.drive_to_point(15 * reversed, 4.25326 - 1.5);
  chassis.drive_timeout = 400;
  chassis.turn_to_point(16.2469 * reversed, -4.37605 - 1.5, 180);
  intakeRev = false;
  chassis.drive_to_point(16.2469 * reversed, -4.37605 - 1.5);

  // Drive to the corner
  Doinker.set(true);
  chassis.drive_timeout = 800;
  chassis.turn_to_point(26 * reversed, 6.5);
  chassis.drive_to_point(26 * reversed, 6.5);

  // Clear the corner
  chassis.turn_timeout = 2000;
  chassis.turn_to_angle(alliance == "blue" ? 150 : 53, 4);
  Doinker.set(false);
  chassis.turn_to_angle(alliance == "blue" ? 46.8294 : 313.17, 12);
  chassis.turn_timeout = 400;

  // Eat the ring in the corner
  intakeSort = true;
  chassis.drive_to_point(29 * reversed, 12);
  vex::wait(200, msec);
  chassis.drive_to_point(28.5 * reversed, 11.5);
  vex::wait(4000, msec);

  // // Drive to the ladder
  // chassis.turn_to_point(12.4361 * reversed, -6.50658, 180);
  // chassis.drive_to_point(12.4361 * reversed, -6.50658);
  // chassis.turn_timeout = 600;
  // chassis.drive_timeout = 2000; 
  // chassis.turn_to_point(-22 * reversed, -21.7566 - 1.5);
  // chassis.drive_to_point(-22 * reversed, -21.7566 - 1.5, 12, 12); 
  // chassis.drive_to_point(-22 * reversed, -21.7566 - 1.5, 7, 7); 
  intakeSort = false;
}

// BASICALLY DONE? 
void auton_skills()
{
  pre_driver = true;

  odom_constants();

  // chassis.drive_kd = 0.01;
  // chassis.turn_kd = 0.01;
  chassis.drive_max_voltage = 12;
  chassis.turn_max_voltage = 10;
  chassis.turn_timeout = 350;
  chassis.drive_timeout = 700;
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

  // Move backwards to align with the next two rings
  chassis.drive_timeout = 800;
  chassis.turn_to_point(41.4149, 59.4474, 180);
  chassis.drive_to_point(41.4149, 59.4474);

  intakeSort = true;
  intakeToFishyAuton = false;

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

  // Get aligned for the mogo
  chassis.turn_to_point(-10.2475, 28.0261, 180);
  chassis.drive_to_point(-10.2475, 28.0261);

  // Turn and drive into the mogo to clamp onto it
  chassis.drive_timeout = 1200;
  chassis.turn_to_point(-20.4184, 14.7244, 180);
  chassis.drive_to_point(-20.4184, 14.7244, 5, 5);
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
  chassis.turn_to_point(57.5896, 80.2526);
  chassis.drive_to_point(57.5896, 80.2526);

  // Push into the corner
  chassis.drive_timeout = 900;
  chassis.turn_to_point(70, 112.997, 180);
  Clamp.set(0);
  chassis.drive_to_point(65, 112.997);

  // Back up
  chassis.drive_to_point(57.5896, 96);

  // Ram the last goal into the corner
  chassis.drive_timeout = 3000;
  chassis.turn_to_point(-48.3737, 122.738, 180);
  chassis.drive_to_point(-48.3737, 122.738, 12, 12);

  // Back up
  chassis.drive_to_point(43.6138, 97.5774);
}