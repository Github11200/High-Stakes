#ifndef PURSUIT_H
#define PURSUIT_H

#include "vex.h"

using namespace vex;
using namespace std;

struct Point
{
  double x;
  double y;
  double speed;

  Point();
  Point(double x, double y, double speed);

  bool hasValues();
};

typedef vector<Point> Path;

class Pursuit
{
private:
  double trackWidth;

  double distance(Point a, Point b);
  double Dot(Point a, Point b);
  int signum(double x);

public:
  Pursuit(double trackWidth);

  Path loadPathFromFile(string fileName);

  Point findClosestPoint(Path path, double currentX, double currentY);
  Point findLookAheadPoint(Path path, double lookAheadDistance, double currentX, double currentY);
  double getCurvature(double lookAheadDistance, double theta, double currentX, double currentY, double lookAheadX, double lookAheadY);

  void followPath(string fileName, double lookAheadDistance, double timeout, bool forwards, double kV, double kA, double kP);
};

#endif