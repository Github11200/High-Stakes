#include "../../include/driver/intake.h"

using namespace vex;

IntakeControl::IntakeControl(int speed)
{
  this->speed = speed;
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
    Intake.spin(vex::directionType::fwd, 1.5, vex::voltageUnits::volt);
    cout << "we're racist" << endl;
    wait(10, vex::timeUnits::msec);
  }
  // by spinning it in reverse, we either toss the ring straight out
  // or have the previous hook come from behind and knock it off
  Intake.spin(vex::directionType::rev, 12, vex::voltageUnits::volt);
  wait(500, vex::timeUnits::msec);
}

void IntakeControl::intake()
{
  while (IntakeButton.pressing())
  {
    OpticalSensor.setLightPower(100, pct); // turn the light on to see the color
    if (shouldEjectRing())
      ejectRing();
    Intake.spin(vex::directionType::fwd, this->speed, vex::voltageUnits::volt);
    wait(10, vex::timeUnits::msec);
  }
}

void IntakeControl::intakeToLadyBrown()
{
  while (LadyBrownLoadButton.pressing())
  {
    while (!ringStopped && LadyBrownLoadButton.pressing())
    {
      Intake.spin(vex::directionType::fwd, 12, vex::voltageUnits::volt);
      wait(60, vex::timeUnits::msec);
      if (Intake.velocity(pct) < 5)
      {
        wait(2, sec);
        if (Intake.velocity(pct) < 5)
        {
          ringStopped = true;
        }
      }
    }
    wait(60, vex::timeUnits::msec);
  }
  ringStopped = false;
}

void IntakeControl::outtake()
{
  while (OuttakeButton.pressing())
  {
    Intake.spin(vex::directionType::rev, this->speed, vex::voltageUnits::volt);
    wait(10, vex::timeUnits::msec);
  }
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

void IntakeControl::intakeAutonTask()
{
  if (intakeSort) // the variable that the autonomous sets to true to spin the intake
  {
    OpticalSensor.setLightPower(100, pct); // turn the light on to see the color
    if (shouldEjectRing())
      ejectRing();
    Intake.spin(vex::directionType::fwd, 12, vex::voltageUnits::volt);
  }
  else if (intakeRev)
  {
    Intake.spin(vex::directionType::rev, 12, vex::voltageUnits::volt);
  }
  else if (intakeToLadyBrownAuton)
  {
    while (!ringStopped)
    {
      Intake.spin(vex::directionType::fwd, 12, vex::voltageUnits::volt);
      if (Intake.velocity(pct) < 5)
      {
        wait(2, sec);
        if (Intake.velocity(pct) < 5)
        {
          ringStopped = true;
        }
      }
      wait(60, vex::timeUnits::msec);
    }
  }
  else if (keepAllianceRing)
  {
    OpticalSensor.setLightPower(100, pct);
    while (true)
    {
      Intake.spin(vex::directionType::fwd, 12, vex::voltageUnits::volt);
      // We've already intaked the ring we don't want and are now intaking the one we do want
      if (OpticalSensor.isNearObject() && this->shouldEjectRing())
        break;
      wait(20, vex::timeUnits::msec);
    }
    wait(200, vex::timeUnits::msec);
    while (!OpticalSensor.isNearObject())
      Intake.spin(vex::directionType::fwd, 6, vex::voltageUnits::volt);
    keepAllianceRing = false;
    Intake.stop(vex::brakeType::coast);
    OpticalSensor.setLightPower(0, pct);
  }
  else
  {
    OpticalSensor.setLightPower(0, pct); // turn off the light to save durability
    Intake.stop(coast);
  }
}
