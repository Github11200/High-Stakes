#ifndef LADY_BROWN_H
#define LADY_BROWN_H

#include "vex.h"

using namespace vex;
using namespace std;

class LadyBrown
{
private:
  PID *ladyBrownPID;
  double loadingPositionAngle;
  double maxVoltage;
  double slewChange;
  double maxAngle;
  double minAngle;

public:
  LadyBrown(double loadingPositionAngle, double maxVoltage, double kP, double kD, double settleError, double settleTime, double timeout, double slewChange, double maxAngle, double minAngle);

  /**
   * @brief Gets the lady brown into loading position
   *
   */
  void loading();

  /**
   * @brief Raises the lady brown as the button is pressed
   *
   */
  void raise();

  /**
   * @brief Lowers the lady brown as another button is pressed
   *
   */
  void lower();

  void autonScore();
};

#endif