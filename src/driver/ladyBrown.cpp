#include "../../include/driver/ladyBrown.h"

using namespace vex;

LadyBrown::LadyBrown(double loadingPositionAngle, double scoringPositionAngle, double maxVoltage, double kP, double kD, double settleError, double settleTime, double timeout, double slewChange, double maxAngle, double minAngle)
{
  this->ladyBrownPID = new PID(0, kP, 0, kD, 0, settleError, settleTime, timeout);
  this->loadingPositionAngle = loadingPositionAngle;
  this->scoringPositionAngle = scoringPositionAngle;
  this->maxVoltage = maxVoltage;
  this->slewChange = slewChange;
  this->maxAngle = maxAngle;
  this->minAngle = minAngle;
}

void LadyBrown::loading()
{
  double error = LadyBrownRotation.angle(vex::rotationUnits::deg) - this->loadingPositionAngle;
  double output = this->ladyBrownPID->compute(error);
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

void LadyBrown::raise()
{
  while (LadyBrownRaiseButton.pressing())
  {
    double voltageOutput = slew(12, LadyBrownMotor.voltage(), this->slewChange);
    LadyBrownMotor.spin(vex::directionType::fwd, voltageOutput, vex::voltageUnits::volt);
    wait(10, vex::timeUnits::msec);
  }
  LadyBrownMotor.stop(vex::brakeType::hold);
}

void LadyBrown::lower()
{
  while (LadyBrownRaiseButton.pressing())
  {
    double voltageOutput = slew(-12, LadyBrownMotor.voltage(), this->slewChange);
    LadyBrownMotor.spin(vex::directionType::fwd, voltageOutput, vex::voltageUnits::volt);
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

void LadyBrown::ladyBrownAutonTask()
{
  if (pre_driver)
  {
    if (intakeToLadyBrownAuton)
    {
      loading();
    }
    else if (ladyBrownScore)
    {
      autonScore(ladyBrownDelay);
      ladyBrownScore = false;
    }
    else
    {
      while (LadyBrownRotation.angle(vex::rotationUnits::deg) > this->minAngle)
      {
        LadyBrownMotor.spin(fwd, -12, volt);
        vex::wait(30, msec);
      }
      LadyBrownMotor.stop(coast);
    }
  }
}