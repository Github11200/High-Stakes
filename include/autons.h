#pragma once
#include "JAR-Template/drive.h"
#include "driver/mogo.h"

extern Drive chassis;

struct ExitConditions
{
  DriveParams driveParams;
  TurnParams turnParams;
  SwingParams swingParams;
};

class Autonomous
{
private:
  color allianceColor;

  thread intakeAutonTask;
  thread ladyBrownAutonTask;
  thread mogoAutonTask;

  DriveParams driveParams;
  TurnParams turnParams;
  SwingParams swingParams;

  DriveParams driveParamsWithMogo;
  TurnParams turnParamsWithMogo;

public:
  Autonomous();
  ~Autonomous();

  vector<Point> mirrorPath(vector<Point> originalPath);

  void setAllianceColor(vex::color allianceColor);

  void solo_awp();
  void positive_six_ring();
  void negative_ring_rush();
  void positive_goal_rush();
  void auton_skills();

  void testing();
};