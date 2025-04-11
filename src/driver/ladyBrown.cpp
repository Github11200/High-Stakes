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
    ringStopped = false;
    LadyBrownMotor.spin(vex::directionType::fwd, voltageOutput / 12 * 100, percent);
    wait(10, vex::timeUnits::msec);
  }
  LadyBrownMotor.stop(vex::brakeType::hold);
}

void LadyBrown::loading()
{
  Intake.spin(vex::directionType::fwd, 12, vex::voltageUnits::volt);
  while (LadyBrownLoadButton.pressing())
  {
    double error = this->loadingPositionAngle - LadyBrownRotation.position(vex::rotationUnits::deg);
    double output = this->ladyBrownPID->compute(error);

    output = clamp(output, -this->maxVoltage, this->maxVoltage);
    LadyBrownMotor.spin(vex::directionType::fwd, output, vex::voltageUnits::volt);

    wait(10, vex::timeUnits::msec);
  }
  cout << "exit" << endl;

  // while (LadyBrownLoadButton.pressing() && !ringStopped && !LadyBrownRaiseButton.pressing() && !LadyBrownForwardButton.pressing())
  // {

  //   Intake.spin(vex::directionType::fwd, 12, vex::voltageUnits::volt);
  //   // if (LadyBrownRotation.angle(vex::rotationUnits::deg) > loadingPositionAngle)
  //   //   LadyBrownMotor.spin(vex::directionType::rev, 1, percent);
  //   // else if (LadyBrownRotation.angle(vex::rotationUnits::deg) < loadingPositionAngle)
  //   //   LadyBrownMotor.spin(vex::directionType::fwd, 1, percent);
  //   // else
  //   //   LadyBrownMotor.stop(hold);

  //   wait(10, vex::timeUnits::msec);
  //   cout << LadyBrownLoadButton.pressing() << !ringStopped << !LadyBrownRaiseButton.pressing() << !LadyBrownForwardButton.pressing() << endl;
  // }
  this->ladyBrownPID->reset();
  LadyBrownMotor.stop(vex::brakeType::hold);
  Intake.stop(vex::brakeType::coast);
}

void LadyBrown::lower(double speed)
{
  cout << "breh" << endl;
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
    LadyBrownMotor.spin(vex::directionType::rev, voltageOutput / 12 * 100, percent);
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
    // while (LadyBrownRotation.position(vex::rotationUnits::deg) > this->minAngle)
    // {
    //   LadyBrownMotor.spin(fwd, -12, volt);
    //   vex::wait(30, msec);
    // }
    LadyBrownMotor.stop(hold);
  }
}