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
};

class Pursuit
{
  Path loadPathFromFile(string fileName);

  Point findClosestPoint(Path path, int currentX, int currentY)
  {
    static int previousStartIndex = 1;

    double minDistance = INT_MAX;
    Point minDistancePoint;

    for (int i = previousStartIndex; i < path.size(); ++i)
    {
      double distance = sqrtl(pow(path[i].x - currentX, 2) + pow(path[i].y - currentY, 2));
      if (distance < minDistance)
        minDistancePoint = path[i];
      minDistance = min(minDistance, distance);
    }

    return minDistancePoint;
  }
};

#endif