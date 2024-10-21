#include "vex.h"

#include "../include/driver/doinker.h"
#include "../include/driver/intake.h"
#include "../include/driver/joystick.h"
#include "../include/driver/mogo.h"
#include "../include/driver/redirect.h"
#include <robot-config.h>

using namespace vex;
competition Competition;

Drive chassis(

    // Specify your drive setup below. There are eight options:
    // ZERO_TRACKER_NO_ODOM, ZERO_TRACKER_ODOM, TANK_ONE_ENCODER, TANK_ONE_ROTATION, TANK_TWO_ENCODER, TANK_TWO_ROTATION, HOLONOMIC_TWO_ENCODER, and HOLONOMIC_TWO_ROTATION
    // For example, if you are not using odometry, put ZERO_TRACKER_NO_ODOM below:
    TANK_ONE_ENCODER,

    // Add the names of your Drive motors into the motor groups below, separated by commas, i.e. motor_group(Motor1,Motor2,Motor3).
    // You will input whatever motor names you chose when you configured your robot using the sidebar configurer, they don't have to be "Motor1" and "Motor2".

    // Left Motors:
    motor_group(FrontLeft, MiddleLeft, BackLeft),

    // Right Motors:
    motor_group(FrontRight, MiddleRight, BackRight),

    // Specify the PORT NUMBER of your inertial sensor, in PORT format (i.e. "PORT1", not simply "1"):
    PORT9,

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
    6,

    // Input the Forward Tracker diameter (reverse it to make the direction switch):
    2.75,

    // Input Forward Tracker center distance (a positive distance corresponds to a tracker on the right side of the robot, negative is left.)
    // For a zero tracker tank drive with odom, put the positive distance from the center of the robot to the right side of the drive.
    // This distance is in inches:
    -2,

    // Input the Sideways Tracker Port, following the same steps as the Forward Tracker Port:
    1,

    // Sideways tracker diameter (reverse to make the direction switch):
    -2.75,

    // Sideways tracker center distance (positive distance is behind the center of the robot, negative is in front):
    5.5

);

int current_auton_selection = 0;
bool pre_match = true;

void setColor(vex::color colour)
{
  Brain.Screen.setPenColor(colour);
  Brain.Screen.setFillColor(colour);
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
  Brain.Screen.printAt(5, 20, "Test");
  Brain.Screen.setFillColor(green);
  Brain.Screen.printAt(165, 20, "One");
  Brain.Screen.setFillColor(black);
  Brain.Screen.printAt(325, 20, "Two");
  Brain.Screen.setFillColor(blue);
  Brain.Screen.printAt(5, 140, "Three");
  Brain.Screen.setFillColor(orange);
  Brain.Screen.printAt(165, 140, "Four");
  Brain.Screen.setFillColor(purple);
  Brain.Screen.printAt(325, 140, "Five");
  Brain.Screen.setFillColor(black);
}

void pre_auton(void)
{
  vexcodeInit();
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
    test(); // This is the default auton, if you don't select from the brain.
    break;  // Change these to be your own auton functions in order to use the auton selector.
  case 1:   // Tap the screen to cycle through autons.
    one();
    break;
  case 2:
    two();
    break;
  case 3:
    three();
    break;
  case 4:
    four();
    break;
  case 5:
    five();
    break;
  }
}

int buttonsWrapper()
{
  // DoinkerControl doinkerControl;
  IntakeControl intakeControl(12, 3);
  MogoControl mogoControl;
  RedirectControl redirectControl(12);

  while (true)
  {
    // // Doinker
    // if (DoinkerButton.pressing())
    //   doinkerControl.toggle();

    // Intake
    if (IntakeButton.pressing())
      intakeControl.intake();
    else if (OuttakeButton.pressing())
      intakeControl.outtake();

    // Mogo
    if (ClampButton.pressing())
      mogoControl.toggle();

    // Redirect
    if (RedirectLiftButton.pressing())
      redirectControl.liftRedirect();
    else if (RedirectLowerButton.pressing())
      redirectControl.lowerRedirect();

    vex::wait(20, vex::timeUnits::msec);
  }

  return 1;
}

int joystickWrapper()
{
  // I don't know what these values do
  Joystick joystickControl(5, 0.02, 0.01, 0.01);

  while (true)
  {
    pair<double, double> values = joystickControl.cheesy();

    Left.spin(vex::directionType::fwd, values.first, vex::voltageUnits::volt);
    Right.spin(vex::directionType::fwd, values.second, vex::voltageUnits::volt);

    vex::wait(20, vex::timeUnits::msec);
  }
  return 1;
}

void usercontrol(void)
{
  pre_match = false;
  Brain.Screen.clearScreen();
  Brain.Screen.printAt(5, 100, "vroom vroom!");
  task buttons = task(buttonsWrapper);
  task joysticks = task(joystickWrapper);

  while (1)
  {
    // Replace this line with chassis.control_tank(); for tank drive
    // or chassis.control_holonomic(); for holo drive.
    // chassis.control_arcade();

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