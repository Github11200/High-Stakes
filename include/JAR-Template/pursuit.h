#ifndef PURSUIT_H
#define PURSUIT_H

#include "vex.h"

using namespace vex;
using namespace std;

typedef vector<Point> Path;

struct Point
{
  double x;
  double y;
  double speed;

  Point()
  {
    this->x = INT_MAX;
    this->y = INT_MAX;
    this->speed = INT_MAX;
  }

  Point(int x, int y, int speed)
  {
    this->x = x;
    this->y = y;
    this->speed = speed;
  }

  bool hasValues()
  {
    return this->speed == INT_MAX && this->x == INT_MAX && this->y == INT_MAX;
  }
};

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