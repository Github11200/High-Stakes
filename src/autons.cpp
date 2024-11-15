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
  chassis.set_turn_exit_conditions(1, 300, 800);
  chassis.set_swing_exit_conditions(1, 300, 800);
}

void odom_constants()
{
  default_constants();
  chassis.drive_max_voltage = 8;
  chassis.drive_settle_error = 3;
}

//TESTED, BUT MOVED FROM ALLIANCE_NEGATIVE_BLUE TO ALLIANCE_NEGATIVE
void alliance_negative(std::string c) {
  int reversed;
  int num;
  if(c == "red") {
    reversed = -1;
    num = 360;
  }
  else if(c == "blue") {
    reversed = 1;
    num = 0;
  }
  odom_constants();
  chassis.set_heading(num + (reversed * 90));

  // Get the Alliance Stake, pushing rings out of the way
  chassis.drive_distance(-9.5, num + (reversed * 90));
  chassis.turn_to_angle(180);  
  chassis.drive_distance(-4, 180);
  Intake.spin(fwd, 12, volt);
  wait(800, msec);
  Intake.stop();

  // Move forwards
  chassis.drive_distance(15.5, 180);

  // Move backwards and clamp onto the goal 
  chassis.turn_to_angle(num + (reversed * 315));
  chassis.drive_distance(-25, num + (reversed * 315));
  chassis.drive_distance(-5, num + (reversed * 315), 3, 3);
  Clamp.set(true);

  // Sweep the two rings on the edge of the line
  chassis.turn_to_angle(num + (reversed * 145));
  chassis.drive_distance(19, num + (reversed * 135));
  Intake.spin(vex::directionType::fwd, 12, vex::voltageUnits::volt);
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

//TESTED, BUT MOVED FROM ALLIANCE_NEGATIVE_BLUE TO ALLIANCE_NEGATIVE
void basic_negative(std::string c) {
  int reversed;
  int num;
  if(c == "red") {
    reversed = -1;
    num = 360;
  }
  else if(c == "blue") {
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

//NOT DONE
void doinker_positive(std::string c) {
  int reversed;
  int num;
  if(c == "red") {
    reversed = -1;
    num = 360;
  }
  else if(c == "blue") {
    reversed = 1;
    num = 0;
  }
  odom_constants();
}

//TESTED, BUT MOVED FROM ALLIANCE_NEGATIVE_BLUE TO ALLIANCE_NEGATIVE
void alliance_positive(std::string c) {
  int reversed;
  int num;
  if(c == "red") {
    reversed = -1;
    num = 360;
  }
  else if(c == "blue") {
    reversed = 1;
    num = 0;
  }

  odom_constants();
  chassis.set_heading(num + (reversed * 270));

  // Get the Alliance Stake, pushing rings out of the way
  chassis.drive_distance(-10.5, num + (reversed * 270));
  chassis.turn_to_angle(180);  
  chassis.drive_distance(-4, 180);
  Intake.spin(fwd, 12, volt);
  wait(800, msec);
  Intake.stop();

  // Move forwards
  chassis.drive_distance(13, 180);

  // Clamp onto the goal
  chassis.turn_to_angle(num + (reversed * 45));
  chassis.drive_distance(-30, num + (reversed * 45), 5, 2); 
  wait(0.2, vex::timeUnits::sec);
  Clamp.set(true); 
  wait(0.5, vex::timeUnits::sec); 

  // Intake the ring on the bottom, closest to the mogo rush goal
  Intake.spin(vex::directionType::fwd, 12, vex::voltageUnits::volt);
  chassis.turn_to_angle(num + (reversed * 270));
  chassis.drive_distance(22);
  wait(2, vex::timeUnits::sec);
  Intake.stop(vex::brakeType::coast);

  //touch the ladder
  chassis.turn_to_angle(num + (reversed * 90));
  chassis.drive_distance(40, num + (reversed * 90), 8, 8);
}

//TESTED, BUT MOVED FROM ALLIANCE_NEGATIVE_BLUE TO ALLIANCE_NEGATIVE
void basic_positive(std::string c) {
  int reversed;
  int num;
  if(c == "red") {
    reversed = -1;
    num = 360;
  }
  else if(c == "blue") {
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
  chassis.drive_distance(-30, num + (reversed * 330), 5, 2); 
  wait(0.2, vex::timeUnits::sec);
  Clamp.set(true); 
  wait(0.5, vex::timeUnits::sec); 

  // Intake the ring on the bottom, closest to the mogo rush goal
  Intake.spin(vex::directionType::fwd, 12, vex::voltageUnits::volt);
  chassis.turn_to_angle(num + (reversed * 270));
  chassis.drive_distance(22);
  wait(2, vex::timeUnits::sec);
  Intake.stop(vex::brakeType::coast);

  //touch the ladder
  chassis.turn_to_angle(num + (reversed * 90));
  chassis.drive_distance(40, num + (reversed * 90), 8, 8);
}

void alliance_negative_blue() {
  alliance_negative("blue");
}

void basic_negative_blue() {
  basic_negative("blue");
}

void doinker_positive_blue() {
  doinker_positive("blue");
}

void alliance_positive_blue() {
  alliance_positive("blue");
}

void basic_positive_blue() {
  basic_positive("blue");
}

void alliance_negative_red() {
  alliance_negative("red");
}

void basic_negative_red() {
  basic_negative("red");
}

void doinker_positive_red() {
  doinker_positive("red");
}

void alliance_positive_red() {
  alliance_positive("red");
}

void basic_positive_red() {
  basic_positive("red");
}

void auton_skills() {
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