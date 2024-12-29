#include "pursuit.h"

using namespace vex;
using namespace std;

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

int Pursuit::signum(double x)
{
  if (x == 0)
    return x;
  if (x < 0)
    return -1;
  if (x > 0)
    return 1;
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

Point Pursuit::findClosestPoint(Path path, double currentX, double currentY)
{
  static int previousStartIndex = 1;
  double minDistance = INT_MAX;
  double distance = 0;

  Point minDistancePoint;

  for (int i = previousStartIndex; i < path.size(); ++i)
  {
    distance = this->distance(path[i], Point(currentX, currentY, 0));
    if (distance < minDistance)
    {
      minDistancePoint = path[i];
      minDistance = i;
    }
    minDistance = min(minDistance, distance);
  }

  // If no point was found then just return the previous one
  if (minDistancePoint.x == INT_MAX)
    return path[previousStartIndex];
  return minDistancePoint;
}

Point Pursuit::findLookAheadPoint(Path path, double lookAheadDistance, double currentX, double currentY)
{
  double r = lookAheadDistance;
  double a = 0;
  double b = 0;
  double c = 0;

  double discriminant = 0;
  double fractionalIndex = 0;
  Point lookAheadPoint;

  double t1 = 0;
  double t2 = 0;

  static double previousFractionalIndex = -1;
  static Point previousLookAheadPoint(0, 0, 0);

  for (int i = 1; i < path.size(); ++i)
  {
    Point rayStart(path[i - 1].x, path[i - 1].y, path[i - 1].speed); // E
    Point rayEnd(path[i].x, path[i].y, path[i].speed);               // L
    Point sphere(currentX, currentY, 0);                             // C

    Point d(rayEnd.x - rayStart.x, rayEnd.y - rayStart.y, 0);
    Point f(rayStart.x - sphere.x, rayStart.y - sphere.y, 0);

    a = this->Dot(d, d);
    b = 2 * this->Dot(f, d);
    c = this->Dot(f, f) - r * r;

    discriminant = b * b - 4 * a * c;

    // No intersection
    if (discriminant < 0)
      continue;

    discriminant = sqrt(discriminant);

    t1 = (-b - discriminant) / (2 * a);
    t2 = (-b + discriminant) / (2 * a);

    fractionalIndex = i + t1;
    if (t1 >= 0 && t1 <= 1 && fractionalIndex >= previousFractionalIndex)
    {
      // t1 is the intersection and it's closer than t2
      lookAheadPoint = Point(rayStart.x + t1 * d.x, rayStart.y + t1 * d.y, 0);
      break;
    }

    fractionalIndex = i + t2;
    if (t2 >= 0 && t2 <= 1 && fractionalIndex >= previousFractionalIndex)
    {
      // t1 doesn't intersect so we either start inside the sphere or completely past it
      lookAheadPoint = Point(rayStart.x + t2 * d.x, rayStart.y + t2 * d.y, 0);
      break;
    }
  }

  previousFractionalIndex = max(previousFractionalIndex, fractionalIndex);
  if (!lookAheadPoint.hasValues())
    return previousLookAheadPoint;

  previousLookAheadPoint = lookAheadPoint;
  return lookAheadPoint;
}

double Pursuit::getCurvature(double lookAheadDistance, double theta, double currentX, double currentY, double lookAheadX, double lookAheadY)
{
  double a = -tan(theta);
  double b = 1;
  double c = tan(theta) * currentX - currentY;

  // horizontal distance to the look ahead point
  double x = abs(a * lookAheadX + b * lookAheadY + c) / sqrt(pow(a, 2) + pow(b, 2));

  double curvature = (2 * x) / pow(lookAheadDistance, 2);

  // what side of the path the robot is on
  int side = this->signum(sin(theta) * (lookAheadX - currentX) - cos(theta) * (lookAheadY - currentY));

  return curvature * (double)side;
}

void Pursuit::followPath(string fileName, double lookAheadDistance, double timeout, bool forwards, double kV, double kA, double kP)
{
  Path path = this->loadPathFromFile(fileName);

  Point lastPointOnPath = path[path.size() - 1];
  Point closestPoint;
  Point lookAheadPoint;

  double curvature;
  double leftTargetVelocity;
  double rightTargetVelocity;

  // double FFLeft;
  // double FFRight;
  // double FBLeft;
  // double FBRight;

  // double previousLeftTargetVelocity = 0;
  // double previousRightTargetVelocity = 0;

  // Used in the wait() function at the end of each loop iteration
  double waitTime = 10;

  for (int i = 0; i < timeout / waitTime; ++i)
  {
    // Get the closest point on the path
    closestPoint = this->findClosestPoint(path, chassis.get_X_position(), chassis.get_Y_position());

    // Get the point where the circle intersects, it doesn't have to be a point that's in the path, just whatever x, y position it intersections the line
    lookAheadPoint = this->findLookAheadPoint(path, lookAheadDistance, chassis.get_X_position(), chassis.get_Y_position());

    // If we're at the end of the path then just set the look ahead point to be the last one
    if (sqrt(pow(lastPointOnPath.x - lookAheadPoint.x, 2) + pow(lastPointOnPath.y - lookAheadPoint.y, 2)) <= lookAheadDistance)
      lookAheadPoint = lastPointOnPath;

    // Get the curvature of the path
    curvature = this->getCurvature(lookAheadDistance, chassis.get_absolute_heading(), chassis.get_X_position(), chassis.get_Y_position(), lookAheadPoint.x, lookAheadPoint.y);

    // Calculate the target speeds for the motors
    leftTargetVelocity = closestPoint.speed * (2 + curvature * this->trackWidth) / 2;
    rightTargetVelocity = closestPoint.speed * (2 - curvature * this->trackWidth) / 2;

    // Spin them in whatever direction based on whether you want to move forward or backward
    if (forwards)
    {
      Left.spin(vex::directionType::fwd, to_volt(leftTargetVelocity), vex::voltageUnits::volt);
      Right.spin(vex::directionType::fwd, to_volt(rightTargetVelocity), vex::voltageUnits::volt);
    }
    else
    {
      Left.spin(vex::directionType::rev, to_volt(leftTargetVelocity), vex::voltageUnits::volt);
      Right.spin(vex::directionType::rev, to_volt(rightTargetVelocity), vex::voltageUnits::volt);
    }

    wait(waitTime, vex::timeUnits::msec);
  }
}