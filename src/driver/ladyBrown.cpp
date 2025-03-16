#include "../../include/driver/ladyBrown.h"

using namespace vex;

LadyBrown::LadyBrown(double loadingPositionAngle, double maxVoltage, double kP, double kD, double settleError, double settleTime, double timeout, double slewChange, double maxAngle, double minAngle)
{
  this->ladyBrownPID = new PID(0, kP, 0, kD, 0, settleError, settleTime, timeout);
  this->loadingPositionAngle = loadingPositionAngle;
  this->maxVoltage = maxVoltage;
  this->slewChange = slewChange;
  this->maxAngle = maxAngle;
  this->minAngle = minAngle;
}

void LadyBrown::loading()
{
  while (!this->ladyBrownPID->is_settled())
  {
    double error = LadyBrownRoation.angle(vex::rotationUnits::deg) - this->loadingPositionAngle;
    double output = this->ladyBrownPID->compute(error);

    output = clamp(output, -this->maxVoltage, this->maxVoltage);
    LadyBrownMotor.spin(vex::directionType::fwd, output, vex::voltageUnits::volt);
    wait(10, vex::timeUnits::msec);
  }
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

void LadyBrown::autonScore()
{
}