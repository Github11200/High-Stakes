#include "vex.h"

using namespace vex;
using namespace std;

Point::Point()
{
  this->x = INT_MAX;
  this->y = INT_MAX;
  this->speed = INT_MAX;
}

Point::Point(double x, double y, double speed)
{
  this->x = x;
  this->y = y;
  this->speed = speed;
}

bool Point::hasValues()
{
  return this->speed == INT_MAX && this->x == INT_MAX && this->y == INT_MAX;
}

Pursuit::Pursuit(double trackWidth)
{
  this->trackWidth = trackWidth;
}

double Pursuit::distance(Point a, Point b)
{
  return sqrt(pow(a.x - b.x, 2) + pow(a.y - b.y, 2));
}

double Pursuit::Dot(Point a, Point b)
{
  return a.x * b.x + a.y * b.y;
}

Point Pursuit::lerp(Point startPoint, Point endPoint, double t)
{
  return startPoint + Point((endPoint - startPoint).x * t, (endPoint - startPoint).y * t, 0);
}

double Pursuit::signum(double x)
{
  return x < 0 ? -1 : 1;
}

/// @brief Reads a text file with the path
/// @param fileName The name of the file
/// @return An array of points with each point having x and y coordinates and a speed
Path Pursuit::loadPathFromFile(string fileName)
{
  ifstream pathFile(fileName);
  Path path;

  string line;
  Point point;

  // Loop through the entire file and get each coordinate and add it to the path
  while (getline(pathFile, line))
  {
    if (line == "endData")
      break;

    string stringNumber = "";
    int resultIndex = 0;
    double pointResult[3];

    for (int i = 0; i < line.size(); ++i)
    {
      // If it's a comma add the number and then increase the resultIndex
      if (line[i] == ',')
      {
        pointResult[resultIndex] = atof(stringNumber.c_str());
        ++resultIndex;
        stringNumber = "";
        continue;
      }
      else if (line[i] == ' ') // Don't do anything for a space
        continue;
      else
        stringNumber += line[i]; // Keep adding characters to get the whole number
    }
    pointResult[2] = atof(stringNumber.c_str());

    // Update the properties of the Point object
    point.x = pointResult[0];
    point.y = pointResult[1];
    point.speed = pointResult[2];

    // Add it to the path
    path.push_back(point);
  }

  pathFile.close();

  return path;
}

int Pursuit::findClosestPoint(Path path, double currentX, double currentY)
{
  double minDistanceIndex = 0;
  double minDistance = INT_MAX;
  double distance = 0;

  for (int i = 0; i < path.size(); ++i)
  {
    distance = this->distance(path[i], Point(currentX, currentY, 0));
    if (distance < minDistance)
    {
      minDistanceIndex = i;
      minDistance = distance;
    }
  }

  return minDistanceIndex;
}

double Pursuit::circleIntersect(Point p1, Point p2, double currentX, double currentY, double lookAheadDistance)
{
  Point d = Point(p2.x - p1.x, p2.y - p1.y, 0);
  Point f = Point(p1.x - currentX, p2.y - currentY, 0);

  double a = this->Dot(d, d);
  double b = 2 * this->Dot(f, d);
  double c = this->Dot(f, f) - lookAheadDistance * lookAheadDistance;
  double discriminant = b * b - 4 * a * c;

  // possible solution
  if (discriminant >= 0)
  {
    discriminant = sqrt(discriminant);

    double t1 = (-b - discriminant) / (2 * a);
    double t2 = (-b + discriminant) / (2 * a);

    if (t2 >= 0 && t2 <= 1)
      return t2;
    if (t1 >= 0 && t1 <= 1)
      return t1;
  }

  return -1;
}

Point Pursuit::findLookAheadPoint(Path path, Point lastLookAhead, int lastLookAheadIndex, int closestPointIndex, double lookAheadDistance, double currentX, double currentY)
{
  // Start as far down the path as possible
  int startIndex = max(closestPointIndex, lastLookAheadIndex);

  for (int i = startIndex; i < path.size() - 1; ++i)
  {
    Point lastPoint = path[i];
    Point currentPoint = path[i + 1];

    double t = this->circleIntersect(lastPoint, currentPoint, currentX, currentY, lookAheadDistance);

    if (t != -1)
    {
      Point lookAheadPoint = this->lerp(lastPoint, currentPoint, t);
      return lookAheadPoint;
    }
  }

  // No point found return last look ahead
  return lastLookAhead;
}

double Pursuit::getCurvature(double theta, double currentX, double currentY, double lookAheadX, double lookAheadY)
{
  // what side of the path the robot is on
  double side = this->signum(sin(theta) * (lookAheadX - currentX) - cos(theta) * (lookAheadY - currentY));

  double a = -tan(theta);
  double c = tan(theta) * currentX - currentY;

  // horizontal distance to the look ahead point
  double x = fabs(a * lookAheadX + lookAheadY + c) / sqrt((a * a) + 1);
  double d = hypot(lookAheadX - currentX, lookAheadY - currentY);

  return side * ((2 * x) / (d * d));
}

double Pursuit::fullStateFeedback(double currentX, double currentY, double targetX, double targetY, double targetVelocity, double currentVelocity, double kP, double kV)
{
  double positionError = hypot(targetX - currentX, targetY - currentY);
  double velocityError = targetVelocity - currentVelocity;

  return (kP * positionError) + (kV * velocityError);
}

void Pursuit::followPath(Path path, double lookAheadDistance, double timeout, bool forwards, double slewGain, double kP, double kV)
{
  Point lastPointOnPath = path[path.size() - 1];
  Point closestPoint;
  Point lookAheadPoint;
  Point lastLookAheadPoint = Point(path[0].x, path[0].y, path[0].speed);
  int lastLookAheadPointIndex = 0;

  double curvature;
  double targetVelocity;
  double previousTargetVelocity = 0;
  double leftTargetVelocity;
  double rightTargetVelocity;

  // Used in the wait() function at the end of each loop iteration
  double waitTime = 20;

  // for (int i = 0; i < path.size(); ++i)
  //   cout << "X: " << path[i].x << "\nY: " << path[i].y << "\nSpeed: " << path[i].speed << endl;

  for (int i = 0; true; ++i)
  {
    // Get the closest point on the path
    int closestPointIndex = this->findClosestPoint(path, chassis.get_X_position(), chassis.get_Y_position());
    closestPoint = path[closestPointIndex];

    if (closestPoint.speed == 0)
    {
      cout << "Exiting Pure Pursuit curve" << endl;
      Left.stop(brake);
      Right.stop(brake);
      break;
    }

    // Get the point where the circle intersects, it doesn't have to be a point that's in the path, just whatever x, y position it intersects the line
    lookAheadPoint = this->findLookAheadPoint(path, lastLookAheadPoint, lastLookAheadPointIndex, closestPointIndex, lookAheadDistance, chassis.get_X_position(), chassis.get_Y_position());

    if (lookAheadPoint == lastLookAheadPoint)
    {
      wait(waitTime, vex::timeUnits::msec);
      continue;
    }

    // Get the curvature of the path
    curvature = this->getCurvature(M_PI / 2 - to_rad(reduce_negative_180_to_180(chassis.Gyro.rotation())), chassis.get_X_position(), chassis.get_Y_position(), lookAheadPoint.x, lookAheadPoint.y);

    // Calculate the velocities
    targetVelocity = closestPoint.speed;
    targetVelocity = this->fullStateFeedback(chassis.get_X_position(), chassis.get_Y_position(), lookAheadPoint.x, lookAheadPoint.y, closestPoint.speed, previousTargetVelocity, kP, kV);
    // targetVelocity = slew(targetVelocity, previousTargetVelocity, slewGain);

    // Calculate the target speeds for the motors
    leftTargetVelocity = (slew(targetVelocity, previousTargetVelocity, slewGain) * (2 + curvature * this->trackWidth) / 2);
    rightTargetVelocity = (slew(targetVelocity, previousTargetVelocity, slewGain) * (2 - curvature * this->trackWidth) / 2);

    previousTargetVelocity = targetVelocity;

    double ratio = max(leftTargetVelocity, rightTargetVelocity) / 100;
    if (ratio > 1)
    {
      leftTargetVelocity /= ratio;
      rightTargetVelocity /= ratio;
    }

    // Spin them in whatever direction based on whether you want to move forward or backward
    if (forwards)
    {
      Left.spin(vex::directionType::fwd, leftTargetVelocity, vex::percentUnits::pct);
      Right.spin(vex::directionType::fwd, rightTargetVelocity, vex::percentUnits::pct);
    }
    else
    {
      Left.spin(vex::directionType::rev, leftTargetVelocity, vex::percentUnits::pct);
      Right.spin(vex::directionType::rev, rightTargetVelocity, vex::percentUnits::pct);
    }

    lastLookAheadPoint = lookAheadPoint;
    lastLookAheadPointIndex = closestPointIndex;

    wait(waitTime, vex::timeUnits::msec);
  }

  cout << "Exited Pure Pursuit Curve. Final Position:" << endl;
  cout << chassis.get_X_position() << ", " << chassis.get_Y_position() << endl;
}