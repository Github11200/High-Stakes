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

  Point operator-(const Point &other)
  {
    return Point(this->x - other.x, this->y - other.y, this->speed);
  }

  Point operator+(const Point &other)
  {
    return Point(this->x + other.x, this->y + other.y, this->speed);
  }

  Point operator+(const double &other)
  {
    return Point(this->x + other, this->y + other, this->speed);
  }

  double operator*(const Point &other)
  {
    return this->x * other.x + this->y * other.y;
  }

  bool operator==(const Point &other)
  {
    return (this->x == other.x) && (this->y == other.y) && (this->speed == other.speed);
  }
};

typedef vector<Point> Path;

class Pursuit
{
private:
  double trackWidth;

  double distance(Point a, Point b);
  double Dot(Point a, Point b);
  Point lerp(Point startPoint, Point endPoint, double t);
  double signum(double x);

public:
  Pursuit(double trackWidth);

  Path loadPathFromFile(string fileName);

  int findClosestPoint(Path path, double currentX, double currentY);

  double circleIntersect(Point p1, Point p2, double currentX, double currentY, double lookAheadDistance);
  Point findLookAheadPoint(Path path, Point lastLookAhead, int lastLookAheadIndex, int closestPointIndex, double lookAheadDistance, double currentX, double currentY);

  double getCurvature(double theta, double currentX, double currentY, double lookAheadX, double lookAheadY);

  void followPath(Path path, double lookAheadDistance, double timeout, bool forwards, double kP, double kI, double kD);
};

#endif