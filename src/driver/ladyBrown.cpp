#include "../../include/driver/ladyBrown.h"
#include "ladyBrown.h"

using namespace vex;

LadyBrown::LadyBrown(double loadingPositionAngle, double maxVoltage, double kP, double kD, double settleError, double settleTime, double timeout, double raisingSlew)
{
  this->ladyBrownPID = new PID(0, kP, 0, kD, 0, settleError, settleTime, timeout);
  this->loadingPositionAngle = loadingPositionAngle;
  this->maxVoltage = maxVoltage;
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
  }
}

void LadyBrown::lower()
{
}