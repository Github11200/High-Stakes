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
  return startPoint + (this->distance(startPoint, endPoint) * t);
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
    else if (t1 >= 0 && t1 <= 1)
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

void test(double X_position, double Y_position, double drive_kp, double drive_ki, double drive_kd)
{
  PID drivePID(hypot(X_position - chassis.get_X_position(), Y_position - chassis.get_Y_position()), drive_kp, drive_ki, drive_kd, 0);
  PID headingPID(reduce_negative_180_to_180(to_deg(atan2(X_position - chassis.get_X_position(), Y_position - chassis.get_Y_position())) - chassis.get_absolute_heading()), 0.3, 0, 0, 0);
  float prev_drive_output = 0;
  float prev_heading_output = 0;
  bool close = false;
  float drive_error = hypot(X_position - chassis.get_X_position(), Y_position - chassis.get_Y_position());
  // The drive error is just equal to the distance between the current and desired points.
  float heading_error = reduce_negative_180_to_180(to_deg(atan2(X_position - chassis.get_X_position(), Y_position - chassis.get_Y_position())) - chassis.get_absolute_heading());
  // This uses atan2(x,y) rather than atan2(y,x) because doing so places 0 degrees on the positive Y axis.
  float drive_output = drivePID.compute(drive_error);

  float heading_scale_factor = cos(to_rad(heading_error));
  drive_output *= heading_scale_factor;
  // The scale factor slows the drive down the more it's facing away from the desired point,
  // and that way the heading correction has time to catch up.
  heading_error = reduce_negative_90_to_90(heading_error);
  // Here we reduce -90 to 90 because this allows the robot to travel backwards if it's easier
  // to do so.
  float heading_output = headingPID.compute(heading_error);

  // This if statement prevents the heading correction from acting up after the robot gets close
  // to being settled.

  drive_output = clamp(drive_output, -fabs(heading_scale_factor) * 12, fabs(heading_scale_factor) * 12);
  heading_output = clamp(heading_output, -12, 12);
  if (drive_error < 3)
  {
    heading_output = 0;
  }

  prev_drive_output = drive_output;
  prev_heading_output = heading_output;
  chassis.drive_with_voltage(drive_output + heading_output, drive_output - heading_output);
}

void Pursuit::followPath(string fileName, double lookAheadDistance, double timeout, bool forwards, double kP, double kI, double kD)
{
  cout << "Starting to follow path..." << endl;
  // Path path = this->loadPathFromFile(fileName);
  Path path = vector<Point>({Point(0, 0, 30.128),
                             Point(-0.077, -1.997, 20),
                             Point(-0.586, -0.788, 39.445),
                             Point(-0.774, 1.202, 32.375),
                             Point(-0.252, 0.972, 30.054),
                             Point(0.391, -0.902, 20),
                             Point(1.687, 0.316, 87.19),
                             Point(2.583, 2.104, 99.795),
                             Point(3.45, 3.906, 97.234),
                             Point(4.339, 5.698, 95.804),
                             Point(5.269, 7.469, 95.256),
                             Point(6.252, 9.21, 93.108),
                             Point(7.291, 10.919, 90.339),
                             Point(8.4, 12.583, 87.482),
                             Point(9.577, 14.199, 84.529),
                             Point(10.829, 15.759, 81.469),
                             Point(12.165, 17.247, 78.29),
                             Point(13.579, 18.661, 74.977),
                             Point(15.072, 19.991, 71.509),
                             Point(16.657, 21.209, 67.865),
                             Point(18.319, 22.32, 64.015),
                             Point(20.054, 23.313, 59.918),
                             Point(21.856, 24.18, 55.518),
                             Point(23.719, 24.905, 50.739),
                             Point(25.633, 25.483, 45.46),
                             Point(27.584, 25.921, 39.482),
                             Point(29.56, 26.222, 32.42),
                             Point(31.552, 26.389, 23.308),
                             Point(33.691, 26.443, 0),
                             Point(33.691, 26.443, 0),
                             Point(53.684, 26.941, 0)});

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

  PID velocityPID(0, kP, kI, kD, 0);

  // Used in the wait() function at the end of each loop iteration
  double waitTime = 20;

  for (int i = 0; true; ++i)
  {
    // Get the closest point on the path
    int closestPointIndex = this->findClosestPoint(path, chassis.get_X_position(), chassis.get_Y_position());
    closestPoint = path[closestPointIndex];

    if (closestPoint.speed == 0)
    {
      cout << "Dipping..." << endl;
      cout << "Robot X: " << chassis.get_X_position() << endl;
      cout << "Robot Y: " << chassis.get_Y_position() << endl;
      break;
    }

    // We've reached the end of the path
    // if (closestPoint.speed == 0)
    // {
    //   cout << "Dipping..." << endl;
    //   cout << "Robot X: " << chassis.get_X_position() << endl;
    //   cout << "Robot Y: " << chassis.get_Y_position() << endl;
    //   break;
    // }

    // cout << "Robot X: " << chassis.get_X_position() << endl;
    // cout << "Robot Y: " << chassis.get_Y_position() << endl;
    // cout << "======================================" << endl;
    // cout << "Closest Point X: " << closestPoint.x << endl;
    // cout << "Closest Point Y: " << closestPoint.y << endl;
    // cout << "======================================" << endl;

    // Get the point where the circle intersects, it doesn't have to be a point that's in the path, just whatever x, y position it intersections the line
    lookAheadPoint = this->findLookAheadPoint(path, lastLookAheadPoint, lastLookAheadPointIndex, closestPointIndex, lookAheadDistance, chassis.get_X_position(), chassis.get_Y_position());

    cout << "Robot X: " << chassis.get_X_position() << endl;
    cout << "Robot Y: " << chassis.get_Y_position() << endl;
    cout << "======================================" << endl;
    cout << "Lookahead Point X: " << lookAheadPoint.x << endl;
    cout << "Lookahead Point Y: " << lookAheadPoint.y << endl;
    cout << "======================================" << endl;

    // Get the curvature of the path
    // cout << "Gyro: " << reduce_negative_180_to_180(chassis.Gyro.rotation()) << endl;
    // cout << "Absolute: " << chassis.get_absolute_heading() << endl;

    curvature = this->getCurvature(chassis.get_absolute_heading(), chassis.get_X_position(), chassis.get_Y_position(), lookAheadPoint.x, lookAheadPoint.y);

    // Calculate the velocities
    targetVelocity = closestPoint.speed;
    targetVelocity = velocityPID.compute(targetVelocity - previousTargetVelocity);
    previousTargetVelocity = targetVelocity;

    // cout << "Curvature: " << curvature << endl;

    // Calculate the target speeds for the motors
    leftTargetVelocity = (targetVelocity * (2 + curvature * this->trackWidth) / 2);
    rightTargetVelocity = (targetVelocity * (2 - curvature * this->trackWidth) / 2);

    double ratio = max(leftTargetVelocity, rightTargetVelocity) / 127;
    if (ratio > 1)
    {
      leftTargetVelocity /= ratio;
      rightTargetVelocity /= ratio;
    }

    // Spin them in whatever direction based on whether you want to move forward or backward
    if (forwards)
    {
      Left.spin(vex::directionType::fwd, to_volt(leftTargetVelocity), vex::voltageUnits::volt);
      Right.spin(vex::directionType::fwd, to_volt(rightTargetVelocity), vex::voltageUnits::volt);

      // test(lookAheadPoint.x, lookAheadPoint.y, kP, kI, kD);
    }
    else
    {
      Left.spin(vex::directionType::rev, to_volt(leftTargetVelocity), vex::voltageUnits::volt);
      Right.spin(vex::directionType::rev, to_volt(rightTargetVelocity), vex::voltageUnits::volt);
    }

    lastLookAheadPoint = lookAheadPoint;
    lastLookAheadPointIndex = closestPointIndex;

    wait(waitTime, vex::timeUnits::msec);
  }
}