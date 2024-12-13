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
    // OpticalSensor.setLightPower(100, pct);
    // if (shouldEjectRing())
    //   ejectRing();
    Intake.spin(vex::directionType::fwd, this->speed, vex::voltageUnits::volt);
    // wait(10, vex::timeUnits::msec);
  }
  // OpticalSensor.setLightPower(0, pct);
  Intake.stop(vex::brakeType::coast);
}

void IntakeControl::intakeToFishy()
{
  OpticalSensor.setLightPower(100, pct);

  // Spin the intake until the optical sensor senses a ring color
  while (!OpticalSensor.isNearObject() && IntakeToFishyButton.pressing())
  {
    Intake.spin(vex::directionType::fwd, 12, vex::voltageUnits::volt);
    wait(10, vex::timeUnits::msec);
  }
  OpticalSensor.setLightPower(0, pct);
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
  static int stuck_time = 0;
  if (pre_driver) // make sure to not stop the intake during driver control
  {
    if (intakeSort) // the variable that the autonomous sets to true to spin the intake
    {
      OpticalSensor.setLightPower(100, pct); // turn the light on to see the color
      if (shouldEjectRing())
        ejectRing();
      Intake.spin(vex::directionType::fwd, 12, vex::voltageUnits::volt);
      cout << abs(Intake.velocity(rpm)) << endl;
      cout << stuck_time << endl;
      if(abs(Intake.velocity(rpm)) < 20){
        stuck_time += 1;
        cout << "e" << endl;
        if(stuck_time >= 30) {
          cout << "f" << endl;
          Intake.spin(vex::directionType::rev, 12, vex::voltageUnits::volt);
          vex::wait(400, msec);
          Intake.spin(vex::directionType::fwd, 12, vex::voltageUnits::volt);
        }
      }
      else {
        stuck_time = 0;
      }
    }
    else if (intakeRev)
    {
      Intake.spin(vex::directionType::rev, 12, vex::voltageUnits::volt);
    }
    else if (!intakeToFishyAuton && !fishing) // don't stop the intake if other systems are running
    {
      OpticalSensor.setLightPower(0, pct); // turn off the light to save durability
      Intake.stop();
    }
  }
}
