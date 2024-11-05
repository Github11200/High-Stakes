#include "../../include/driver/intake.h"

using namespace vex;

IntakeControl::IntakeControl(int speed, int numberOfSecondsToRedirect)
{
  this->speed = speed;
  this->numberOfSecondsToRedirect = numberOfSecondsToRedirect;
}

void IntakeControl::intake()
{
  while (IntakeButton.pressing())
    Intake.spin(vex::directionType::fwd, this->speed, vex::voltageUnits::volt);
  Intake.stop(vex::brakeType::coast);
}

void IntakeControl::intakeToFishy()
{
  // Spin the intake until the optical sensor senses a ring color
  while ((!(OpticalSensor.color() == red) || !(OpticalSensor.color() == blue)) && IntakeToFishyButton.pressing())
    Intake.spin(vex::directionType::fwd, 12, vex::voltageUnits::volt);
  Intake.stop(vex::brakeType::brake);
}

void IntakeControl::outtake()
{
  while (OuttakeButton.pressing())
    Intake.spin(vex::directionType::rev, this->speed, vex::voltageUnits::volt);
  Intake.stop(vex::brakeType::coast);
}
