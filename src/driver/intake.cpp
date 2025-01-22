#include "../../include/driver/intake.h"

using namespace vex;

IntakeControl::IntakeControl(int speed, int numberOfSecondsToRedirect, double originalHueValue)
{
  this->speed = speed;
  this->numberOfSecondsToRedirect = numberOfSecondsToRedirect;
  this->originalHueValue = originalHueValue;
}

bool IntakeControl::shouldEjectRing()
{
  bool isNear = OpticalSensor.isNearObject();
  color ringColor = NULL;
  if (isNear)
  {
    if (OpticalSensor.hue() > 100 && OpticalSensor.hue() < 280)
      ringColor = blue;

    else if (OpticalSensor.hue() < 30)
      ringColor = red;

    if (alliance == "red")
      return ringColor == blue;

    else if (alliance == "blue")
      return ringColor == red;

    else if (alliance == "skills")
      return false;
  }
  return false;
}

void IntakeControl::ejectRing()
{
  while (OpticalSensor.isNearObject())
  { // spin it until the ring is not near
    // once it's not near, it's probably in the perfect place for ejection
    Intake.spin(vex::directionType::fwd, 12, vex::voltageUnits::volt);
    Hooks.spin(vex::directionType::fwd, 12, vex::voltageUnits::volt);
    wait(10, vex::timeUnits::msec);
  }
  // by spinning it in reverse, we either toss the ring straight out
  // or have the previous hook come from behind and knock it off
  Intake.spin(vex::directionType::fwd, 12, vex::voltageUnits::volt);
  Hooks.spin(vex::directionType::fwd, 12, vex::voltageUnits::volt);
  wait(500, vex::timeUnits::msec);
}

void IntakeControl::intake()
{
  while (IntakeButton.pressing())
  {
    Intake.spin(vex::directionType::fwd, this->speed, vex::voltageUnits::volt);
    Hooks.spin(vex::directionType::fwd, this->speed, vex::voltageUnits::volt);
    wait(10, vex::timeUnits::msec);
  }
  Intake.stop(vex::brakeType::coast);
  Hooks.stop(vex::brakeType::coast);
}

void IntakeControl::intakeToFrog()
{
  OpticalSensor.setLightPower(100, pct);
  // Spin the intake until the optical sensor senses a ring color
  while (!OpticalSensor.isNearObject() && IntakeToFrogButton.pressing() && !ringStopped)
  {
    Intake.spin(vex::directionType::fwd, 12, vex::voltageUnits::volt);
    Hooks.spin(vex::directionType::fwd, 7, vex::voltageUnits::volt);
    wait(60, vex::timeUnits::msec);
  }
  ringStopped = true;
  OpticalSensor.setLightPower(0, pct);
  wait(0.1, vex::timeUnits::sec);
  // if (!IntakeToFrogButton.pressing())
  // {
  //   Hooks.spin(vex::directionType::rev, 6, vex::voltageUnits::volt);
  //   wait(0.3, vex::timeUnits::sec);
  // }
  Intake.stop(vex::brakeType::brake);
  Hooks.stop(vex::brakeType::brake);
}

void IntakeControl::outtake()
{
  while (OuttakeButton.pressing())
  {
    Intake.spin(vex::directionType::rev, this->speed, vex::voltageUnits::volt);
    Hooks.spin(vex::directionType::rev, 12, vex::voltageUnits::volt);
    wait(10, vex::timeUnits::msec);
    if (Hooks.velocity(rpm) < 2 && OuttakeButton.pressing())
    {
      vex::wait(1, sec);
      while (Hooks.velocity(rpm) < 2 && OuttakeButton.pressing())
      {
        Doinker.set(true);
        vex::wait(1, sec);
      }
    }
  }
  wait(10, vex::timeUnits::msec);
  Intake.stop(vex::brakeType::coast);
  Hooks.stop(vex::brakeType::coast);
}

int IntakeControl::hue_difference(int hue1, int hue2)
{
  if ((abs(hue1 - hue2) < abs((hue1 - 360) - hue2)) && (abs(hue1 - hue2) < abs(hue1 - (hue2 - 360))))
  {
    return hue1 - hue2;
  }
  else if (abs(hue1 - (hue2 - 360)) < abs((hue1 - 360) - hue2))
  {
    return hue1 - (hue2 - 360);
  }
  else
  {
    return (hue1 - 360) - hue2;
  }
}

void IntakeControl::intakeToFrogAutonTask()
{
  if (pre_driver)
  {
    if (intakeToFrogAuton)
    {
      // Spin the intake until the optical sensor senses a ring color
      while (!OpticalSensor.isNearObject())
      {
        Intake.spin(vex::directionType::fwd, 8, vex::voltageUnits::volt);
        wait(10, vex::timeUnits::msec);
      }
      Intake.stop(vex::brakeType::brake);
      intakeToFrogAuton = false;
    }
  }
}

void IntakeControl::colorSortingAutonTask()
{
  static int stuck_time = 0;
  if (pre_driver) // make sure to not stop the intake during driver control
  {
    if (intakeSort) // the variable that the autonomous sets to true to spin the intake
    {
      OpticalSensor.setLightPower(100, pct); // turn the light on to see the color
      if (shouldEjectRing())
        ejectRing();
      Intake.spin(vex::directionType::fwd, 12, vex::voltageUnits::volt);
      if (abs(Intake.velocity(rpm)) < 5)
      {
        stuck_time += 1;
        cout << "e" << endl;
        if (stuck_time >= 15)
        {
          cout << "f" << endl;
          Intake.spin(vex::directionType::rev, 12, vex::voltageUnits::volt);
          vex::wait(400, msec);
          Intake.spin(vex::directionType::fwd, 12, vex::voltageUnits::volt);
        }
      }
      else
      {
        stuck_time = 0;
      }
    }
    else if (intakeRev)
    {
      Intake.spin(vex::directionType::rev, 12, vex::voltageUnits::volt);
    }
    else if (!intakeToFrogAuton) // don't stop the intake if other systems are running
    {
      OpticalSensor.setLightPower(0, pct); // turn off the light to save durability
      Intake.stop();
    }
  }
}
