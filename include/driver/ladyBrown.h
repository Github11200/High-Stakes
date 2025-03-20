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
  double scoringPositionAngle;
  double allianceStakeAngle;
  double maxVoltage;
  double slewChange;
  double maxAngle;
  double minAngle;

public:
  LadyBrown(double loadingPositionAngle, double scoringPositionAngle, double allianceStakeAngle, double maxVoltage, double kP, double kD, double settleError, double settleTime, double timeout, double slewChange, double maxAngle, double minAngle);

  /// @brief Gets the lady brown into loading position
  void loading();

  /// @brief Raises the lady brown as the button is pressed
  void raise();

  /// @brief Lowers the lady brown as another button is pressed
  void lower();

  /// @brief Gets the lady brown into scoring position
  void score();

  /// @brief Scores the ring after a set number of milliseconds
  /// @param delay The number of milliseconds you want to delay by
  void autonScore(int delay);

  /// @brief Gets the lady brown into scoring position
  void allianceStakeScore();

  /// @brief Scores the ring after a set number of milliseconds
  /// @param delay The number of milliseconds you want to delay by
  void allianceStakeAutonScore(int delay);

  void ladyBrownAutonTask();
};

#endif