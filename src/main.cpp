#include "vex.h"

#include "../include/driver/intake.h"
#include "../include/driver/joystick.h"
#include "../include/driver/mogo.h"
#include "../include/driver/fishy.h"
#include <robot-config.h>
#include <string>
#include <sstream>

using namespace vex;
competition Competition;

Drive chassis(

    // Specify your drive setup below. There are eight options:
    // ZERO_TRACKER_NO_ODOM, ZERO_TRACKER_ODOM, TANK_ONE_ENCODER, TANK_ONE_ROTATION, TANK_TWO_ENCODER, TANK_TWO_ROTATION, HOLONOMIC_TWO_ENCODER, and HOLONOMIC_TWO_ROTATION
    // For example, if you are not using odometry, put ZERO_TRACKER_NO_ODOM below:
    TANK_ONE_ROTATION,

    // Add the names of your Drive motors into the motor groups below, separated by commas, i.e. motor_group(Motor1,Motor2,Motor3).
    // You will input whatever motor names you chose when you configured your robot using the sidebar configurer, they don't have to be "Motor1" and "Motor2".

    // Left Motors:
    motor_group(FrontLeft, MiddleLeft, BackLeft),

    // Right Motors:
    motor_group(FrontRight, MiddleRight, BackRight),

    // Specify the PORT NUMBER of your inertial sensor, in PORT format (i.e. "PORT1", not simply "1"):
    PORT8,

    // Input your wheel diameter. (4" omnis are actually closer to 4.125"):
    2.75,

    // External ratio, must be in decimal, in the format of input teeth/output teeth.
    // If your motor has an 84-tooth gear and your wheel has a 60-tooth gear, this value will be 1.4.
    // If the motor drives the wheel directly, this value is 1:
    0.75,

    // Gyro scale, this is what your gyro reads when you spin the robot 360 degrees.
    // For most cases 360 will do fine here, but this scale factor can be very helpful when precision is necessary.
    360,

    /*---------------------------------------------------------------------------*/
    /*                                  PAUSE!                                   */
    /*                                                                           */
    /*  The rest of the drive constructor is for robots using POSITION TRACKING. */
    /*  If you are not using position tracking, leave the rest of the values as  */
    /*  they are.                                                                */
    /*---------------------------------------------------------------------------*/

    // If you are using ZERO_TRACKER_ODOM, you ONLY need to adjust the FORWARD TRACKER CENTER DISTANCE.

    // FOR HOLONOMIC DRIVES ONLY: Input your drive motors by position. This is only necessary for holonomic drives, otherwise this section can be left alone.
    // LF:      //RF:
    PORT1, -PORT2,

    // LB:      //RB:
    PORT3, -PORT4,

    // If you are using position tracking, this is the Forward Tracker port (the tracker which runs parallel to the direction of the chassis).
    // If this is a rotation sensor, enter it in "PORT1" format, inputting the port below.
    // If this is an encoder, enter the port as an integer. Triport A will be a "1", Triport B will be a "2", etc.
    PORT6,

    // Input the Forward Tracker diameter (reverse it to make the direction switch):
    2.71,

    // Input Forward Tracker center distance (a positive distance corresponds to a tracker on the right side of the robot, negative is left.)
    // For a zero tracker tank drive with odom, put the positive distance from the center of the robot to the right side of the drive.
    // This distance is in inches:
    0,

    // Input the Sideways Tracker Port, following the same steps as the Forward Tracker Port:
    1,

    // Sideways tracker diameter (reverse to make the direction switch):
    2.75,

    // Sideways tracker center distance (positive distance is behind the center of the robot, negative is in front):
    0

);

int current_auton_selection = 0;
float chassisTemp = 0;
float intakeTemp = 0;
bool pre_match = true;

void setColor(vex::color colour)
{
  Brain.Screen.setPenColor(colour);
  Brain.Screen.setFillColor(colour);
}

std::string ToString(int val)
{
  std::stringstream stream;
  stream << val;
  return stream.str();
}

void updateScreen()
{
  setColor(red);
  Brain.Screen.drawRectangle(0, 0, 160, 120);
  setColor(blue);
  Brain.Screen.drawRectangle(0, 120, 160, 120);
  setColor(green);
  Brain.Screen.drawRectangle(160, 0, 160, 120);
  setColor(orange);
  Brain.Screen.drawRectangle(160, 120, 160, 120);
  setColor(black);
  Brain.Screen.drawRectangle(320, 0, 160, 120);
  setColor(purple);
  Brain.Screen.drawRectangle(320, 120, 160, 120);

  Brain.Screen.setPenColor(white);
  Brain.Screen.setFillColor(red);
  Brain.Screen.printAt(5, 20, "Two ring blue side");
  Brain.Screen.setFillColor(green);
  Brain.Screen.printAt(165, 20, "Two ring red side");
  Brain.Screen.setFillColor(black);
  Brain.Screen.printAt(325, 20, "Solo AWP blue");
  Brain.Screen.setFillColor(blue);
  Brain.Screen.printAt(5, 140, "Solo AWP red");
  Brain.Screen.setFillColor(orange);
  Brain.Screen.printAt(165, 140, "Auton skills");
  Brain.Screen.setFillColor(purple);
  Brain.Screen.printAt(325, 140, "Six");
  Brain.Screen.setFillColor(black);
}

void senseTemp()
{
  Brain.Screen.setPenColor(white);
  chassisTemp = FrontLeft.temperature();
  intakeTemp = FrontLeft.temperature();
  Brain.Screen.printAt(5, 20, "Chassis Temp");
  Brain.Screen.printAt(5, 40, ToString(chassisTemp).c_str());
  Brain.Screen.printAt(165, 20, "Intake Temp");
  Brain.Screen.printAt(165, 40, ToString(intakeTemp).c_str());
}

void pre_auton(void)
{
  vexcodeInit();
  FishyMech.setPosition(0, degrees);
  OpticalSensor.gestureDisable();
  alliance = 1;
  chassis.set_coordinates(0, 0, 0);
  default_constants();
  updateScreen();
  while (pre_match)
  { // Changing the names below will only change their names on the
    switch (current_auton_selection)
    {
    case 0:
      Brain.Screen.printAt(5, 100, "SELECTED");
      break;
    case 1:
      Brain.Screen.printAt(165, 100, "SELECTED");
      break;
    case 2:
      Brain.Screen.printAt(325, 100, "SELECTED");
      break;
    case 3:
      Brain.Screen.printAt(5, 220, "SELECTED");
      break;
    case 4:
      Brain.Screen.printAt(165, 220, "SELECTED");
      break;
    case 5:
      Brain.Screen.printAt(325, 220, "SELECTED");
      break;
    }
    if (Brain.Screen.pressing())
    {
      while (Brain.Screen.pressing())
      {
      }
      if (Brain.Screen.xPosition() < 160)
      {
        if (Brain.Screen.yPosition() < 120)
        {
          current_auton_selection = 0;
        }
        else
        {
          current_auton_selection = 3;
        }
      }
      else if (Brain.Screen.xPosition() < 320)
      {
        if (Brain.Screen.yPosition() < 120)
        {
          current_auton_selection = 1;
        }
        else
        {
          current_auton_selection = 4;
        }
      }
      else
      {
        if (Brain.Screen.yPosition() < 120)
        {
          current_auton_selection = 2;
        }
        else
        {
          current_auton_selection = 5;
        }
      }
      Brain.Screen.clearScreen();
      updateScreen();
    }
    task::sleep(20);
  }
}

void autonomous(void)
{
  pre_match = false;
  switch (current_auton_selection)
  {
  case 0:
    three_ring_ladder_blue(); // This is the default auton, if you don't select from the brain.
    //alliance = blue;          // Change these to be your own auton functions in order to use the auton selector.
    break;                    // Tap the screen to cycle through autons.
  case 1:
    three_ring_ladder_red();
    //alliance = red;
    break;
  case 2:
    solo_awp_blue();
    //alliance = blue;
    break;
  case 3:
    solo_awp_red();
    //alliance = red;
    break;
  case 4:
    auton_skills();
    //alliance = red;
    break;
  case 5:
    test();
    break;
  }
}

int buttonsWrapper()
{
  IntakeControl intakeControl(12, 3, OpticalSensor.hue());
  MogoControl mogoControl;
  Fishy fishyControl(12);

  while (true)
  {
    // double hue = OpticalSensor.hue();
    // bool isNear = OpticalSensor.isNearObject();
    // if (hue > 100 && isNear)
    //   cout << "Blue: " << hue << endl;
    // else if (hue < 30 && isNear)
    //   cout << "Red" << endl;
    // else
    //   cout << "Nothing :)" << endl;

    // Doinker
    if (DoinkerButton.pressing())
    {
      Doinker.set(!Doinker.value());
      while (DoinkerButton.pressing())
      {
        vex::wait(20, vex::timeUnits::msec);
      }
    }

    // Intake
    if (OuttakeButton.pressing())
      intakeControl.outtake();
    else if (IntakeButton.pressing())
      intakeControl.intake();
    else if (IntakeToFishyButton.pressing())
      intakeControl.intakeToFishy();

    // Mogo
    if (ClampButton.pressing())
      mogoControl.toggle();

    // Redirect
    if (FishyResetButton.pressing())
      fishyControl.resetPosition();
    else if (FishyLiftButton.pressing())
      fishyControl.liftFishy();
    else if (FishyLowerButton.pressing())
      fishyControl.lowerFishy();

    vex::wait(60, vex::timeUnits::msec);
  }

  return 1;
}

int joystickWrapper()
{
  // I don't know what these values do
  Joystick joystickControl(5, 0.02, 0.01, 0.01);

  while (true)
  {
    // pair<double, double> values = joystickControl.cheesy();

    // Left.spin(vex::directionType::fwd, values.first, vex::voltageUnits::volt);
    // Right.spin(vex::directionType::fwd, values.second, vex::voltageUnits::volt);

    // vex::wait(20, vex::timeUnits::msec);
  }
  return 1;
}

void usercontrol(void)
{
  pre_match = false;

  OpticalSensor.setLightPower(100, vex::percentUnits::pct);

  Brain.Screen.clearScreen();
  Brain.Screen.printAt(5, 100, "vroom vroom!");
  task buttons = task(buttonsWrapper);

  // lv_init();
  /* Set basic Widget attributes */
  // lv_obj_set_size(btn1, 100, 50); /* Set a button's size */
  // lv_obj_set_pos(btn1, 20, 30);   /* Set a button's position */
  // solo_awp_blue();

  while (1)
  {
    // Replace this line with chassis.control_tank(); for tank drive
    // or chassis.control_holonomic(); for holo drive.
    chassis.control_arcade();

    wait(20, msec);
  }
}

int main()
{
  Competition.autonomous(autonomous);
  Competition.drivercontrol(usercontrol);

  pre_auton();

  while (true)
  {
    wait(100, msec);
  }
}