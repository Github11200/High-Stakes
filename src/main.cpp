#include "vex.h"
#include "../include/driver/intake.h"
#include "../include/driver/ladyBrown.h"
#include "../include/driver/joystick.h"
#include "../include/driver/mogo.h"
#include <robot-config.h>
#include <string>
#include <sstream>

using namespace vex;
competition Competition;

Drive chassis(

    // Specify your drive setup below. There are eight options:
    // ZERO_TRACKER_NO_ODOM, ZERO_TRACKER_ODOM, TANK_ONE_ENCODER, TANK_ONE_ROTATION, TANK_TWO_ENCODER, TANK_TWO_ROTATION, HOLONOMIC_TWO_ENCODER, and HOLONOMIC_TWO_ROTATION
    // For example, if you are not using odometry, put ZERO_TRACKER_NO_ODOM below:
    TANK_TWO_ROTATION,

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
    (3600 - (19.6 + 18.3 + 18.7 + 21.51 + 19.16 + 20.35) / 6) / 10,

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
    PORT2,

    // Input the Forward Tracker diameter (reverse it to make the direction switch):
    1.98298,

    // Input Forward Tracker center distance (a positive distance corresponds to a tracker on the right side of the robot, negative is left.)
    // For a zero tracker tank drive with odom, put the positive distance from the center of the robot to the right side of the drive.
    // This distance is in inches:
    0,

    // Input the Sideways Tracker Port, following the same steps as the Forward Tracker Port:
    PORT10,

    // Sideways tracker diameter (reverse to make the direction switch):
    1.98298,

    // Sideways tracker center distance (positive distance is behind the center of the robot, negative is in front):
    -0.6

);

Autonomous autonomousClass;
int current_auton_selection = 4;
float chassisTemp = 0;
float intakeTemp = 0;
bool ladyBrownGoDown = false;
bool pre_match = true;
bool ringStopped = false;
int descoreTime = 0;

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
  if (alliance == "blue")
  {
    setColor(blue);
  }
  else if (alliance == "red")
  {
    setColor(red);
  }
  else if (alliance == "skills")
  {
    setColor(black);
  }
  Brain.Screen.drawRectangle(0, 0, 160, 120);
  setColor(blue);
  Brain.Screen.drawRectangle(0, 120, 160, 120);
  setColor(green);
  Brain.Screen.drawRectangle(160, 0, 160, 120);
  setColor(orange);
  Brain.Screen.drawRectangle(160, 120, 160, 120);
  setColor(red);
  Brain.Screen.drawRectangle(320, 0, 160, 120);
  setColor(purple);
  Brain.Screen.drawRectangle(320, 120, 160, 120);

  Brain.Screen.setPenColor(white);
  if (alliance == "blue")
  {
    Brain.Screen.setFillColor(blue);
    Brain.Screen.printAt(5, 20, "BLUE ALLIANCE");
  }
  else if (alliance == "red")
  {
    Brain.Screen.setFillColor(red);
    Brain.Screen.printAt(5, 20, "RED ALLIANCE");
  }
  else if (alliance == "skills")
  {
    Brain.Screen.setFillColor(black);
    Brain.Screen.printAt(5, 20, "SKILLS");
  }
  Brain.Screen.setFillColor(green);
  Brain.Screen.printAt(165, 20, "solo awp");
  Brain.Screen.setFillColor(cyan);
  Brain.Screen.printAt(325, 20, "positive six ring");
  Brain.Screen.setFillColor(red);
  Brain.Screen.printAt(5, 140, "negative six ring");
  Brain.Screen.setFillColor(orange);
  Brain.Screen.printAt(165, 140, "positive mogo rush");
  Brain.Screen.setFillColor(purple);
  Brain.Screen.printAt(325, 140, "testing");
  Brain.Screen.setFillColor(black);
}

void pre_auton(void)
{
  vexcodeInit();

  OpticalSensor.gestureDisable();
  OpticalSensor.setLightPower(0, pct);

  // CHANGE THIS FOR MATCHES
  alliance = "red";

  chassis.calibrate_robot();

  vex::wait(3000, vex::timeUnits::msec);

  updateScreen();

  while (pre_match)
  {
    switch (current_auton_selection)
    {
    case 0:
      Brain.Screen.printAt(165, 100, "SELECTED");
      break;
    case 1:
      Brain.Screen.printAt(325, 100, "SELECTED");
      break;
    case 2:
      Brain.Screen.printAt(5, 220, "SELECTED");
      break;
    case 3:
      Brain.Screen.printAt(165, 220, "SELECTED");
      break;
    case 4:
      Brain.Screen.printAt(325, 220, "SELECTED");
      break;
    }
    if (Brain.Screen.pressing())
    {
      while (Brain.Screen.pressing())
      {
        vex::wait(10, msec);
      }
      if (Brain.Screen.xPosition() < 160)
      {
        if (Brain.Screen.yPosition() < 120)
        {
          if (alliance == "skills")
          {
            alliance = "red";
          }
          else if (alliance == "red")
          {
            alliance = "blue";
          }
          else if (alliance == "blue")
          {
            alliance = "skills";
          }
        }
        else
        {
          current_auton_selection = 2;
        }
      }
      else if (Brain.Screen.xPosition() < 320)
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
      else
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
      Brain.Screen.clearScreen();
      updateScreen();
    }
    // task::sleep(20);
    vex::wait(20, msec);
  }
}

void autonomous(void)
{
  pre_match = false;
  autonomousClass.setAllianceColor(vex::color::red);

  if (alliance == "skills")
  {
    autonomousClass.auton_skills();
  }
  else if (true)
  {
    switch (current_auton_selection)
    {
    case 0:
      autonomousClass.solo_awp();
      break;
    case 1:
      autonomousClass.positive_six_ring();
      break;
    case 2:
      autonomousClass.negative_ring_rush();
      break;
    case 3:
      autonomousClass.positive_goal_rush();
      break;
    case 4:
      autonomousClass.testing();
      break;
    default:
      break;
    }
  }
}

int buttonsWrapper()
{
  IntakeControl intakeControl(12, 3, OpticalSensor.hue());
  LadyBrown ladyBrown(20, 180, 220, 90, 12, 2, 3, 5, 0.1, 2000, 10000000, 270, 5);
  MogoControl mogoControl;

  while (true)
  {
    chassis.control_arcade();

    // Doinker
    if (RightDoinkerButton.pressing())
    {
      RightDoinker.set(!RightDoinker.value());
      while (RightDoinkerButton.pressing())
      {
        vex::wait(20, vex::timeUnits::msec);
      }
    }
    if (LeftDoinkerButton.pressing())
    {
      LeftDoinker.set(!LeftDoinker.value());
      while (LeftDoinkerButton.pressing())
      {
        vex::wait(20, vex::timeUnits::msec);
      }
    }

    // Intake
    if (OuttakeButton.pressing())
      intakeControl.outtake();
    else if (IntakeButton.pressing())
      intakeControl.intake();
    else if (LadyBrownLoadButton.pressing())
    {
      intakeControl.intakeToLadyBrown();
      ladyBrown.loading();
      ladyBrownGoDown = true;
    }
    else
    {
      OpticalSensor.setLightPower(0, pct);
      Intake.stop(brake);
    }

    // Lady Brown
    if (LadyBrownRaiseButton.pressing())
      ladyBrown.raise();
    else if (!LadyBrownLoadButton.pressing() && ladyBrownGoDown)
      ladyBrown.lower();
    else
      LadyBrownMotor.stop(hold);

    // Mogo
    if (ClampButton.pressing())
      mogoControl.toggle();

    if (ClampResetButton.pressing())
      mogoControl.reset();

    if (RedAllianceButton.pressing())
      alliance = "red";
    else if (BlueAllianceButton.pressing())
      alliance = "blue";

    // if (Controller.ButtonUp.pressing() || Controller.ButtonRight.pressing() || Controller.ButtonLeft.pressing() || Controller.ButtonB.pressing() || Controller.ButtonDown.pressing())
    // {
    //   if (Controller.ButtonUp.pressing())
    //     cout << "chassis.drive_to_point(" << chassis.get_X_position() << "*reversed, " << chassis.get_Y_position() << ");" << endl;
    //   else if (Controller.ButtonRight.pressing())
    //     cout << "chassis.turn_to_point(" << chassis.get_X_position() << ", " << chassis.get_Y_position() << ", 180);" << endl;
    //   else if (Controller.ButtonLeft.pressing())
    //   {
    //     cout << "chassis.turn_to_point(" << chassis.get_X_position() << ", " << chassis.get_Y_position() << ");" << endl;
    //     cout << "chassis.drive_to_point(" << chassis.get_X_position() << ", " << chassis.get_Y_position() << ");" << endl;
    //   }
    //   else if (Controller.ButtonB.pressing())
    //   {
    //     cout << "chassis.turn_to_point(" << chassis.get_X_position() << ", " << chassis.get_Y_position() << ", 180);" << endl;
    //     cout << "chassis.drive_to_point(" << chassis.get_X_position() << ", " << chassis.get_Y_position() << ");" << endl;
    //   }
    //   else if (Controller.ButtonDown.pressing())
    //   {
    //     cout << "chassis.turn_to_angle(" << chassis.get_absolute_heading() << ");" << endl;
    //   }
    //   wait(100, vex::timeUnits::msec);
    // }

    vex::wait(50, vex::timeUnits::msec);
  }

  return 1;
}

int joystickWrapper()
{
  while (true)
  {
    chassis.control_arcade();
    wait(20, vex::timeUnits::msec);
  }
  return 1;
}

void usercontrol(void)
{
  // Auton testing code start
  // wait(3000, msec);
  // auton_skills();

  // intakeSort = false;
  // Intake.stop(vex::brakeType::coast);
  // Hooks.stop(vex::brakeType::coast);
  // Auton testing code end

  pre_match = false;
  ringStopped = false;
  OpticalSensor.setLightPower(0, vex::percentUnits::pct);

  chassis.stop_position_track_task();
  task buttons = task(buttonsWrapper);

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