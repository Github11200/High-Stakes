#include "../../include/driver/ladyBrown.h"

using namespace vex;

LadyBrown::LadyBrown(double loadingPositionAngle, double scoringPositionAngle, double allianceStakeAngle, double holdUpAngle, double maxVoltage, double kP, double kD, double settleError, double settleTime, double timeout, double slewChange, double maxAngle, double minAngle)
{
  this->ladyBrownPID = new PID(0, kP, 0, kD, 0, settleError, settleTime, timeout);
  this->loadingPositionAngle = loadingPositionAngle;
  this->scoringPositionAngle = scoringPositionAngle;
  this->allianceStakeAngle = allianceStakeAngle;
  this->holdUpAngle = holdUpAngle;
  this->maxVoltage = maxVoltage;
  this->slewChange = slewChange;
  this->maxAngle = maxAngle;
  this->minAngle = minAngle;
}

void LadyBrown::autonLoading()
{
  while (!this->ladyBrownPID->is_settled())
  {
    double error = LadyBrownRotation.angle(vex::rotationUnits::deg) - this->loadingPositionAngle;
    double output = this->ladyBrownPID->compute(error);

    output = clamp(output, -this->maxVoltage, this->maxVoltage);
    LadyBrownMotor.spin(vex::directionType::fwd, output, vex::voltageUnits::volt);
    wait(10, vex::timeUnits::msec);
  }
  this->ladyBrownPID->reset();
}

void LadyBrown::raise(double speed)
{
  while (LadyBrownRaiseButton.pressing() || LadyBrownForwardButton.pressing())
  {
    double voltageOutput = slew(speed, LadyBrownMotor.voltage(), this->slewChange);
    ladyBrownGoDown = false;
    ringStopped = false;
    LadyBrownMotor.spin(vex::directionType::fwd, voltageOutput, vex::voltageUnits::volt);
    wait(10, vex::timeUnits::msec);
  }
  LadyBrownMotor.stop(vex::brakeType::hold);
}

void LadyBrown::loading()
{
  Intake.spin(vex::directionType::fwd, 12, vex::voltageUnits::volt);
  while (LadyBrownLoadButton.pressing() && !LadyBrownRaiseButton.pressing())
  {
    double error = this->loadingPositionAngle - LadyBrownRotation.angle(vex::rotationUnits::deg);
    double output = this->ladyBrownPID->compute(error);

    output = clamp(output, -this->maxVoltage, this->maxVoltage);
    LadyBrownMotor.spin(vex::directionType::fwd, output, vex::voltageUnits::volt);

    if (this->ladyBrownPID->is_settled())
      break;

    wait(10, vex::timeUnits::msec);
  }

  LadyBrownMotor.stop(vex::brakeType::hold);
  Intake.spin(vex::directionType::fwd, 12, vex::voltageUnits::volt);
  wait(0.1, vex::timeUnits::sec);
  while (LadyBrownLoadButton.pressing() && !ringStopped && !LadyBrownRaiseButton.pressing())
  {
    if (Intake.velocity(pct) < 5)
    {
      wait(1, sec);
      if (Intake.velocity(pct) < 5)
      {
        ringStopped = true;
        ladyBrownGoDown = false;
      }
    }
  }
  this->ladyBrownPID->reset();
  LadyBrownMotor.stop(vex::brakeType::hold);
  Intake.stop(vex::brakeType::coast);
}

void LadyBrown::lower(double speed)
{
  cout << "breh" << endl;
  while (!LadyBrownRaiseButton.pressing() && !LadyBrownLoadButton.pressing() && LadyBrownLowerButton.pressing())
  {
    double voltageOutput = slew(speed, LadyBrownMotor.voltage(), this->slewChange);
    ladyBrownGoDown = false;
    ringStopped = false;
    cout << voltageOutput << endl;
    LadyBrownMotor.spin(vex::directionType::rev, voltageOutput, vex::voltageUnits::volt);
    wait(10, vex::timeUnits::msec);
  }
  LadyBrownMotor.stop(vex::brakeType::hold);
}

void LadyBrown::score()
{
  while (!this->ladyBrownPID->is_settled())
  {
    double error = LadyBrownRotation.angle(vex::rotationUnits::deg) - this->scoringPositionAngle;
    double output = this->ladyBrownPID->compute(error);

    output = clamp(output, -this->maxVoltage, this->maxVoltage);
    LadyBrownMotor.spin(vex::directionType::fwd, output, vex::voltageUnits::volt);
    wait(10, vex::timeUnits::msec);
  }
  this->ladyBrownPID->reset();
}

void LadyBrown::autonScore(int delay)
{
  static int staticDelay = delay; // Make it static so it persists
  static LadyBrown *ladyBrownPtr = this;

  thread ladyBrownDelayThread([]()
                              {
                            wait(staticDelay, vex::timeUnits::msec);
                            ladyBrownPtr->score(); });
}

void LadyBrown::allianceStakeScore()
{
  while (!this->ladyBrownPID->is_settled())
  {
    double error = LadyBrownRotation.angle(vex::rotationUnits::deg) - this->allianceStakeAngle;
    double output = this->ladyBrownPID->compute(error);

    output = clamp(output, -this->maxVoltage, this->maxVoltage);
    LadyBrownMotor.spin(vex::directionType::fwd, output, vex::voltageUnits::volt);
    wait(10, vex::timeUnits::msec);
  }
  this->ladyBrownPID->reset();
}

void LadyBrown::allianceStakeAutonScore(int delay)
{
  static int staticDelay = delay; // Make it static so it persists
  static LadyBrown *ladyBrownPtr = this;

  thread ladyBrownDelayThread([]()
                              {
                            wait(staticDelay, vex::timeUnits::msec);
                            ladyBrownPtr->allianceStakeScore(); });
}

void LadyBrown::holdUp()
{
  while (!this->ladyBrownPID->is_settled())
  {
    double error = LadyBrownRotation.angle(vex::rotationUnits::deg) - this->holdUpAngle;
    double output = this->ladyBrownPID->compute(error);

    output = clamp(output, -this->maxVoltage, this->maxVoltage);
    LadyBrownMotor.spin(vex::directionType::fwd, output, vex::voltageUnits::volt);
    wait(10, vex::timeUnits::msec);
  }
  this->ladyBrownPID->reset();
}

void LadyBrown::ladyBrownAutonTask()
{
  if (intakeToLadyBrownAuton)
  {
    autonLoading();
  }
  else if (ladyBrownScore)
  {
    autonScore(ladyBrownDelay);
    ladyBrownScore = false;
  }
  else if (ladyBrownAllianceStakeScore)
  {
    allianceStakeAutonScore(ladyBrownDelay);
    ladyBrownAllianceStakeScore = false;
  }
  else
  {
    while (LadyBrownRotation.angle(vex::rotationUnits::deg) > this->minAngle)
    {
      LadyBrownMotor.spin(fwd, -12, volt);
      vex::wait(30, msec);
    }
    LadyBrownMotor.stop(hold);
  }
}