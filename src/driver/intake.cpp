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
  if(isNear) {
    cout << "near!" << endl;
    if (OpticalSensor.hue() > 100 && OpticalSensor.hue() < 280)
    {
      cout << "blue ring" << endl;
      Brain.Screen.clearScreen();
      Brain.Screen.setFillColor(blue);
      Brain.Screen.drawRectangle(0, 0, 200, 200);
      Brain.Screen.setCursor(0, 0);
      ringColor = blue;
    }
    else if (OpticalSensor.hue() < 30)
    {
      cout << "red ring" << endl;
      Brain.Screen.clearScreen();
      Brain.Screen.setFillColor(red);
      Brain.Screen.drawRectangle(0, 0, 200, 200);
      Brain.Screen.setCursor(0, 0);
      ringColor = red;
    }
    cout << alliance << endl;
    if(alliance == "red"){
      cout << "red" << endl;
      return ringColor == blue;
    }
    else if(alliance == "blue") {
      cout << "blue" << endl;
      return ringColor == red;
    }
    else if(alliance == "skills") {
      cout << "no alliance selected" << endl;
      return false;
    }
  }
  return false;
}

void IntakeControl::ejectRing()
{
  while(OpticalSensor.isNearObject()) {
    Intake.spin(vex::directionType::fwd, 10, vex::voltageUnits::volt);
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
  }
  OpticalSensor.setLightPower(0, pct);
  Intake.stop(vex::brakeType::coast);
}

void IntakeControl::intakeToFishy()
{
  // Spin the intake until the optical sensor senses a ring color
  while (!OpticalSensor.isNearObject() && IntakeToFishyButton.pressing())
    Intake.spin(vex::directionType::fwd, 12, vex::voltageUnits::volt);
  Intake.stop(vex::brakeType::brake);
}

void IntakeControl::outtake()
{
  while (OuttakeButton.pressing())
    Intake.spin(vex::directionType::rev, this->speed, vex::voltageUnits::volt);
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