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
  if (OpticalSensor.hue() - this->originalHueValue > 20)
  {
    Brain.Screen.clearScreen();
    Brain.Screen.setFillColor(blue);
    Brain.Screen.drawRectangle(0, 0, 200, 200);
    Brain.Screen.setCursor(0, 0);
    Brain.Screen.print("Original value: %d", this->originalHueValue);
    ringColor = blue;
  }
  else if (OpticalSensor.hue() - this->originalHueValue < -20)
  {
    Brain.Screen.clearScreen();
    Brain.Screen.setFillColor(red);
    Brain.Screen.drawRectangle(0, 0, 200, 200);
    Brain.Screen.setCursor(0, 0);
    Brain.Screen.print("Original value: %d", this->originalHueValue);
    ringColor = red;
  }
  return ringColor != NULL && ringColor != alliance && isNear;
}

void IntakeControl::ejectRing()
{
  Intake.spin(vex::directionType::fwd, 12, vex::voltageUnits::volt);
  wait(170, vex::timeUnits::msec);
  Intake.stop(vex::brakeType::brake);
  wait(1000, vex::timeUnits::msec);
}

void IntakeControl::intake()
{
  while (IntakeButton.pressing())
  {
    if (this->shouldEjectRing())
      this->ejectRing();
    Intake.spin(vex::directionType::fwd, this->speed, vex::voltageUnits::volt);
  }
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