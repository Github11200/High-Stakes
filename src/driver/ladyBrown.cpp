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

void LadyBrown::resetLadyBrownPID()
{
  this->ladyBrownPID->reset();
}

void LadyBrown::autonLoading()
{
  while (true)
  {
    double error = this->loadingPositionAngle - LadyBrownRotation.position(vex::rotationUnits::deg);
    double output = this->ladyBrownPID->compute(error);

    output = clamp(output, -12, 12);
    LadyBrownMotor.spin(vex::directionType::fwd, output, vex::voltageUnits::volt);
    wait(10, vex::timeUnits::msec);
  }
  this->ladyBrownPID->reset();
  LadyBrownMotor.stop(vex::brakeType::hold);
}

void LadyBrown::raise(double speed)
{
  while (LadyBrownRaiseButton.pressing() || LadyBrownForwardButton.pressing())
  {
    double voltageOutput = slew(speed, LadyBrownMotor.voltage(), this->slewChange);
    ringStopped = false;
    LadyBrownMotor.spin(vex::directionType::fwd, voltageOutput, volt);
    wait(10, vex::timeUnits::msec);
  }
  LadyBrownMotor.stop(vex::brakeType::hold);
  Intake.stop(vex::brakeType::coast);
}

void LadyBrown::loading()
{
  while (LadyBrownLoadButton.pressing())
  {
    double error = this->loadingPositionAngle - LadyBrownRotation.position(vex::rotationUnits::deg);
    if (LadyBrownRotation.position(vex::rotationUnits::deg) > 208)
    {
      error = -abs(error);
    }
    double output = this->ladyBrownPID->compute(error);

    output = clamp(output, -this->maxVoltage, this->maxVoltage);
    LadyBrownMotor.spin(vex::directionType::fwd, output, vex::voltageUnits::volt);

    if (IntakeButton.pressing())
      Intake.spin(vex::directionType::fwd, 12, vex::voltageUnits::volt);
    else
      Intake.stop(vex::brakeType::coast);

    wait(10, vex::timeUnits::msec);
  }

  this->ladyBrownPID->reset();
  LadyBrownMotor.stop(vex::brakeType::hold);
}

void LadyBrown::lower(double speed)
{
  while (!LadyBrownRaiseButton.pressing() && !LadyBrownLoadButton.pressing() && LadyBrownLowerButton.pressing())
  {
    cout << LadyBrownRotation.angle() << endl;
    if (LadyBrownRotation.angle() <= 5 || LadyBrownRotation.angle() > 330)
    {
      LadyBrownMotor.stop(vex::brakeType::brake);
      cout << "done lowering" << endl;
      break;
    }
    double voltageOutput = slew(speed, LadyBrownMotor.voltage(), this->slewChange);
    ringStopped = false;
    LadyBrownMotor.spin(vex::directionType::rev, voltageOutput, volt);
    wait(10, vex::timeUnits::msec);
  }
  if (!LadyBrownLowerButton.pressing())
  {
    LadyBrownMotor.stop(vex::brakeType::hold);
  }
}

void LadyBrown::score()
{
  while (!this->ladyBrownPID->is_settled())
  {
    double error = this->scoringPositionAngle - LadyBrownRotation.angle(vex::rotationUnits::deg);
    double output = this->ladyBrownPID->compute(error);

    output = clamp(output, -this->maxVoltage, this->maxVoltage);
    LadyBrownMotor.spin(vex::directionType::fwd, output, vex::voltageUnits::volt);
    wait(10, vex::timeUnits::msec);
  }
  this->ladyBrownPID->reset();
}

void LadyBrown::descore()
{
  while (LadyBrownDescoreButton.pressing())
  {
    double error = 166 - LadyBrownRotation.position(vex::rotationUnits::deg);
    double output = this->ladyBrownPID->compute(error);

    output = clamp(output, -this->maxVoltage, this->maxVoltage);
    LadyBrownMotor.spin(vex::directionType::fwd, output, vex::voltageUnits::volt);

    wait(10, vex::timeUnits::msec);
  }
  this->ladyBrownPID->reset();
  LadyBrownMotor.stop(vex::brakeType::hold);
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
    double error = this->allianceStakeAngle - LadyBrownRotation.position(vex::rotationUnits::deg);
    double output = this->ladyBrownPID->compute(error);

    output = clamp(output, -this->maxVoltage, this->maxVoltage);
    LadyBrownMotor.spin(vex::directionType::fwd, output, vex::voltageUnits::volt);
    wait(10, vex::timeUnits::msec);
  }
  this->ladyBrownPID->reset();
  LadyBrownMotor.stop(vex::brakeType::hold);
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
  while (true)
  {
    double error = this->holdUpAngle - LadyBrownRotation.angle(vex::rotationUnits::deg);
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
  }
  else if (!ladyBrownAllianceStakeScore)
  {
    autonLoading();
  }
  else
  {
    // while (LadyBrownRotation.position(vex::rotationUnits::deg) > this->minAngle)
    // {
    //   LadyBrownMotor.spin(fwd, -12, volt);
    //   vex::wait(30, msec);
    // }
  }
}