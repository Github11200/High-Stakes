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

Point Pursuit::findClosestPoint(Path path, double currentX, double currentY)
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

  return path[minDistanceIndex];
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
  static int previousStartIndex = 1;

  for (int i = previousStartIndex; i < path.size(); ++i)
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
    fractionalIndex = i + t2;

    if (t2 >= 0 && t2 <= 1)
    {
      // t1 doesn't intersect so we either start inside the sphere or completely past it
      lookAheadPoint = Point(rayStart.x + t2 * d.x, rayStart.y + t2 * d.y, 0);
      previousLookAheadPoint = lookAheadPoint;
      previousStartIndex = i;
      return lookAheadPoint;
    }
    else if (t1 >= 0 && t1 <= 1)
    {
      // t1 is the intersection and it's closer than t2
      lookAheadPoint = Point(rayStart.x + t1 * d.x, rayStart.y + t1 * d.y, 0);
      previousLookAheadPoint = lookAheadPoint;
      previousStartIndex = i;
      return lookAheadPoint;
    }
  }

  // previousFractionalIndex = max(previousFractionalIndex, fractionalIndex);
  // // if (!lookAheadPoint.hasValues())
  // //   return previousLookAheadPoint;

  return path[previousStartIndex];
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

void Pursuit::followPath(string fileName, double lookAheadDistance, double timeout, bool forwards, double kV, double kA, double kP)
{
  cout << "Starting to follow path..." << endl;
  // Path path = this->loadPathFromFile(fileName);
  Path path = vector<Point>({Point(0, -0.04, 83.459),
                             Point(0.155, 1.953, 83.373),
                             Point(0.513, 3.92, 80.587),
                             Point(1.075, 5.838, 77.372),
                             Point(1.829, 7.689, 74.018),
                             Point(2.756, 9.46, 70.505),
                             Point(3.839, 11.141, 66.806),
                             Point(5.063, 12.722, 62.891),
                             Point(6.407, 14.203, 58.714),
                             Point(7.855, 15.581, 54.217),
                             Point(9.4, 16.851, 49.311),
                             Point(11.025, 18.015, 43.86),
                             Point(12.722, 19.073, 37.627),
                             Point(14.484, 20.018, 30.131),
                             Point(16.304, 20.847, 20),
                             Point(17.363, 21.266, 0)});

  Point lastPointOnPath = path[path.size() - 1];
  Point closestPoint;
  Point lookAheadPoint;

  double curvature;
  double targetVelocity;
  double previousTargetVelocity = 0;
  double leftTargetVelocity;
  double rightTargetVelocity;

  // Used in the wait() function at the end of each loop iteration
  double waitTime = 20;

  for (int i = 0; true; ++i)
  {
    // Get the closest point on the path
    closestPoint = this->findClosestPoint(path, chassis.get_X_position(), chassis.get_Y_position());

    // cout << "Robot X: " << chassis.get_X_position() << endl;
    // cout << "Robot Y: " << chassis.get_Y_position() << endl;
    // cout << "======================================" << endl;
    // cout << "Closest Point X: " << closestPoint.x << endl;
    // cout << "Closest Point Y: " << closestPoint.y << endl;
    // cout << "======================================" << endl;

    // Get the point where the circle intersects, it doesn't have to be a point that's in the path, just whatever x, y position it intersections the line
    lookAheadPoint = this->findLookAheadPoint(path, lookAheadDistance, chassis.get_X_position(), chassis.get_Y_position());

    // cout << "Robot X: " << chassis.get_X_position() << endl;
    // cout << "Robot Y: " << chassis.get_Y_position() << endl;
    // cout << "======================================" << endl;
    // cout << "Lookahead Point X: " << lookAheadPoint.x << endl;
    // cout << "Lookahead Point Y: " << lookAheadPoint.y << endl;
    // cout << "======================================" << endl;

    // If we're at the end of the path then just set the look ahead point to be the last one
    if (hypot(lookAheadPoint.x - lastPointOnPath.x, lookAheadPoint.y - lastPointOnPath.y) <= lookAheadDistance)
    {
      cout << "Dipping..." << endl;
      lookAheadPoint = lastPointOnPath;
      return;
    }

    // Get the curvature of the path
    curvature = this->getCurvature(chassis.get_absolute_heading(), chassis.get_X_position(), chassis.get_Y_position(), lookAheadPoint.x, lookAheadPoint.y);
    targetVelocity = closestPoint.speed;
    // targetVelocity = slew_func(targetVelocity, previousTargetVelocity, 0.2);
    previousTargetVelocity = targetVelocity;

    cout << "Curvature: " << curvature << endl;

    // Calculate the target speeds for the motors
    leftTargetVelocity = (targetVelocity * (2 + curvature * this->trackWidth) / 2);
    rightTargetVelocity = (targetVelocity * (2 - curvature * this->trackWidth) / 2);

    // cout << "Left: " << to_volt(leftTargetVelocity) << endl;
    // cout << "Right: " << to_volt(rightTargetVelocity) << endl;

    // Spin them in whatever direction based on whether you want to move forward or backward
    if (forwards)
    {
      Left.spin(vex::directionType::fwd, to_volt(leftTargetVelocity * 0.2), vex::voltageUnits::volt);
      Right.spin(vex::directionType::fwd, to_volt(rightTargetVelocity * 0.2), vex::voltageUnits::volt);
    }
    else
    {
      Left.spin(vex::directionType::rev, to_volt(leftTargetVelocity), vex::voltageUnits::volt);
      Right.spin(vex::directionType::rev, to_volt(rightTargetVelocity), vex::voltageUnits::volt);
    }

    wait(waitTime, vex::timeUnits::msec);
  }
}