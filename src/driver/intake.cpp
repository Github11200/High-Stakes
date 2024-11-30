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
  {
    Intake.spin(vex::directionType::fwd, 12, vex::voltageUnits::volt);
    wait(10, vex::timeUnits::msec);
  }
  Intake.spin(vex::directionType::rev, 12, vex::voltageUnits::volt);
  wait(500, vex::timeUnits::msec);
}

void IntakeControl::intake()
{
  while (IntakeButton.pressing())
  {
    OpticalSensor.setLightPower(100, pct);
    if (shouldEjectRing())
      ejectRing();
    Intake.spin(vex::directionType::fwd, this->speed, vex::voltageUnits::volt);
    wait(10, vex::timeUnits::msec);
  }
  OpticalSensor.setLightPower(0, pct);
  Intake.stop(vex::brakeType::coast);
}

void IntakeControl::intakeToFishy()
{
  // Spin the intake until the optical sensor senses a ring color
  while (!OpticalSensor.isNearObject() && IntakeToFishyButton.pressing())
  {
    Intake.spin(vex::directionType::fwd, 12, vex::voltageUnits::volt);
    wait(10, vex::timeUnits::msec);
  }
  Intake.stop(vex::brakeType::brake);
}

void IntakeControl::outtake()
{
  while (OuttakeButton.pressing())
  {
    Intake.spin(vex::directionType::rev, this->speed, vex::voltageUnits::volt);
    wait(10, vex::timeUnits::msec);
  }
  wait(10, vex::timeUnits::msec);
  Intake.stop(vex::brakeType::coast);
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

void IntakeControl::intakeToFishyAutonTask()
{
  if (pre_driver)
  {
    if (intakeToFishyAuton)
    {
      // Spin the intake until the optical sensor senses a ring color
      while (!OpticalSensor.isNearObject())
      {
        Intake.spin(vex::directionType::fwd, 8, vex::voltageUnits::volt);
        wait(10, vex::timeUnits::msec);
      }
      Intake.stop(vex::brakeType::brake);
      intakeToFishyAuton = false;
    }
  }
}

void IntakeControl::colorSortingAutonTask()
{
  if (pre_driver)
  {
    if (intakeSort)
    {
      OpticalSensor.setLightPower(100, pct);
      if (shouldEjectRing())
        ejectRing();
      Intake.spin(vex::directionType::fwd, 12, vex::voltageUnits::volt);
    }
    else if (!intakeToFishyAuton && !fishing)
    {
      OpticalSensor.setLightPower(0, pct);
      Intake.stop();
    }
  }
}
