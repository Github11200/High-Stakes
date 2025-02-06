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
  Hooks.spin(vex::directionType::rev, 12, vex::voltageUnits::volt);
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
    Hooks.spin(vex::directionType::fwd, 8, vex::voltageUnits::volt);
    wait(60, vex::timeUnits::msec);
  }
  OpticalSensor.setLightPower(0, pct);
  ringStopped = true;
  if (OpticalSensor.isNearObject())
  {
    wait(0.075, vex::timeUnits::sec);
  }
  Intake.stop(vex::brakeType::brake);
  Hooks.stop(vex::brakeType::brake);
}

void IntakeControl::outtake()
{
  while (OuttakeButton.pressing())
  {
    Intake.spin(vex::directionType::rev, this->speed, vex::voltageUnits::volt);
    Hooks.spin(vex::directionType::rev, 5, vex::voltageUnits::volt);
    wait(10, vex::timeUnits::msec);
  }
  Intake.stop(vex::brakeType::coast);
  Hooks.stop(vex::brakeType::coast);
  Doinker.set(false);
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
      OpticalSensor.setLightPower(100, pct);
      // Spin the intake until the optical sensor senses a ring color
      while (!OpticalSensor.isNearObject())
      {
        Intake.spin(vex::directionType::fwd, 12, vex::voltageUnits::volt);
        Hooks.spin(vex::directionType::fwd, 4, vex::voltageUnits::volt);
        wait(60, vex::timeUnits::msec);
      }
      OpticalSensor.setLightPower(0, pct);
      wait(0.52, vex::timeUnits::sec);

      Hooks.stop(vex::brakeType::brake);
      intakeToFrogAuton = false;
    }
  }
}

void IntakeControl::colorSortingAutonTask()
{
  if (pre_driver) // make sure to not stop the intake during driver control
  {
    if (intakeSort) // the variable that the autonomous sets to true to spin the intake
    {
      OpticalSensor.setLightPower(100, pct); // turn the light on to see the color
      if (shouldEjectRing())
        ejectRing();
      Intake.spin(vex::directionType::fwd, 12, vex::voltageUnits::volt);
      Hooks.spin(vex::directionType::fwd, 12, vex::voltageUnits::volt);
    }
    else if (intakeRev)
    {
      Intake.spin(vex::directionType::rev, 12, vex::voltageUnits::volt);
      Hooks.stop();
    }
    else if (!intakeToFrogAuton) // don't stop the intake if other systems are running
    {
      OpticalSensor.setLightPower(0, pct); // turn off the light to save durability
      Hooks.stop();
    }
  }
}
