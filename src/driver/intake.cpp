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
  while (RedirectLimitSwitch.pressing() && IntakeToRedirectButton.pressing())
    Intake.spin(vex::directionType::fwd, 12, vex::voltageUnits::volt);

  Intake.setVelocity(45, pct);
  Intake.spinFor(fwd, 300, msec);
  Intake.setVelocity(30, pct);
  Intake.spinFor(fwd, 300, msec);
  Intake.setVelocity(15, pct);
  Intake.spinFor(fwd, 200, msec);
  // while (!RedirectLimitSwitch.pressing() && IntakeToRedirectButton.pressing())
  //   Intake.spin(vex::directionType::fwd, 5, vex::voltageUnits::volt);

  // Now that it's at the top, spin it in reverse to put it into the redirect
  while (IntakeToRedirectButton.pressing())
    Intake.spin(vex::directionType::rev, 6, vex::voltageUnits::volt);
  Intake.stop();

  vex::wait(20, vex::timeUnits::msec);
}

void IntakeControl::outtake()
{
  while (OuttakeButton.pressing())
    Intake.spin(vex::directionType::rev, this->speed, vex::voltageUnits::volt);
  Intake.stop(vex::brakeType::coast);
}
