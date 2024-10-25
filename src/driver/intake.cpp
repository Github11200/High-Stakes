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

void IntakeControl::intakeToRedirect()
{
  // Intake the ring until it reaches the limit switch
  while (RedirectLimitSwitch.pressing() && IntakeToRedirectButton.pressing())
    Intake.spin(vex::directionType::fwd, 11, vex::voltageUnits::volt);

  while (!RedirectLimitSwitch.pressing() && IntakeToRedirectButton.pressing())
    Intake.spin(vex::directionType::fwd, 6, vex::voltageUnits::volt);

  // Now that it's at the top, spin it in reverse to put it into the redirect
  while (IntakeToRedirectButton.pressing())
    Intake.spin(vex::directionType::rev, 10, vex::voltageUnits::volt);
  Intake.stop();

  vex::wait(20, vex::timeUnits::msec);
}

void IntakeControl::outtake()
{
  while (OuttakeButton.pressing())
    Intake.spin(vex::directionType::rev, this->speed, vex::voltageUnits::volt);
  Intake.stop(vex::brakeType::coast);
}
