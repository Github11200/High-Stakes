#include "../../include/driver/intake.h"

using namespace vex;

IntakeControl::IntakeControl(int speed, int numberOfSecondsToRedirect)
{
  this->speed = speed;
  this->numberOfSecondsToRedirect = numberOfSecondsToRedirect;
}

bool IntakeControl::shouldEjectRing()
{
  bool isNear = OpticalSensor.isNearObject();
  color ringColor = NULL;
  if (OpticalSensor.hue() > 100)
  {
    Brain.Screen.clearScreen();
    Brain.Screen.setFillColor(blue);
    Brain.Screen.drawRectangle(0, 0, 200, 200);
    ringColor = blue;
  }
  else if (OpticalSensor.hue() < 38)
  {
    Brain.Screen.clearScreen();
    Brain.Screen.setFillColor(red);
    Brain.Screen.drawRectangle(0, 0, 200, 200);
    ringColor = red;
  }
  return ringColor != NULL && ringColor != alliance && isNear;
}

void IntakeControl::ejectRing()
{
  Intake.spin(vex::directionType::fwd, 12, vex::voltageUnits::volt);
  wait(180, vex::timeUnits::msec);
  Intake.stop(vex::brakeType::brake);
  wait(2000, vex::timeUnits::msec);
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