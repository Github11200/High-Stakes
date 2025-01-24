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

void test(double X_position, double Y_position, double drive_kp, double drive_ki, double drive_kd)
{
  PID drivePID(hypot(X_position - chassis.get_X_position(), Y_position - chassis.get_Y_position()), drive_kp, drive_ki, drive_kd, 0);
  PID headingPID(reduce_negative_180_to_180(to_deg(atan2(X_position - chassis.get_X_position(), Y_position - chassis.get_Y_position())) - chassis.get_absolute_heading()), 0.2, 0, 0, 0);
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

  drive_output = clamp(drive_output, -fabs(heading_scale_factor) * 3, fabs(heading_scale_factor) * 3);
  heading_output = clamp(heading_output, -3, 3);
  heading_output = slew(heading_output, prev_heading_output, 0.1);
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
  Path path = vector<Point>({Point(0.239, -0.239, 83.209),
                             Point(1.576, 1.248, 82.666),
                             Point(2.914, 2.735, 82.12),
                             Point(4.251, 4.223, 81.57),
                             Point(5.588, 5.71, 81.017),
                             Point(6.925, 7.197, 80.459),
                             Point(8.263, 8.684, 79.898),
                             Point(9.6, 10.171, 79.333),
                             Point(10.937, 11.658, 78.763),
                             Point(12.275, 13.146, 78.19),
                             Point(13.612, 14.633, 77.612),
                             Point(14.949, 16.12, 77.03),
                             Point(16.286, 17.607, 76.444),
                             Point(17.624, 19.094, 75.853),
                             Point(18.961, 20.581, 75.257),
                             Point(20.298, 22.068, 74.657),
                             Point(21.636, 23.556, 74.052),
                             Point(22.973, 25.043, 73.442),
                             Point(24.53, 24.613, 72.945),
                             Point(26.158, 23.451, 72.326),
                             Point(27.768, 22.265, 71.701),
                             Point(29.357, 21.05, 71.07),
                             Point(30.916, 19.798, 70.434),
                             Point(32.434, 18.495, 69.792),
                             Point(33.893, 17.128, 69.145),
                             Point(35.274, 15.682, 68.491),
                             Point(36.546, 14.139, 67.831),
                             Point(37.697, 12.504, 67.164),
                             Point(38.765, 10.813, 66.491),
                             Point(39.819, 9.113, 65.811),
                             Point(40.916, 7.441, 65.123),
                             Point(42.078, 5.813, 64.429),
                             Point(43.3, 4.23, 63.727),
                             Point(44.576, 2.69, 63.017),
                             Point(45.895, 1.187, 62.298),
                             Point(46.711, -0.438, 61.638),
                             Point(46.259, -2.385, 60.904),
                             Point(45.678, -4.299, 60.161),
                             Point(44.935, -6.154, 59.409),
                             Point(43.997, -7.919, 58.647),
                             Point(42.838, -9.546, 57.875),
                             Point(41.466, -10.999, 57.093),
                             Point(39.919, -12.265, 56.3),
                             Point(38.255, -13.374, 55.495),
                             Point(36.532, -14.389, 54.678),
                             Point(34.787, -15.366, 53.849),
                             Point(33.047, -16.353, 53.007),
                             Point(31.333, -17.382, 52.151),
                             Point(29.657, -18.474, 51.281),
                             Point(28.032, -19.639, 50.396),
                             Point(26.463, -20.879, 49.495),
                             Point(24.953, -22.191, 48.577),
                             Point(23.503, -23.567, 47.642),
                             Point(21.749, -23.415, 46.803),
                             Point(19.936, -22.574, 45.832),
                             Point(18.217, -21.553, 44.84),
                             Point(16.619, -20.353, 43.826),
                             Point(15.159, -18.988, 42.787),
                             Point(13.844, -17.482, 41.723),
                             Point(12.657, -15.874, 40.63),
                             Point(11.576, -14.191, 39.507),
                             Point(10.562, -12.467, 38.351),
                             Point(9.577, -10.727, 37.16),
                             Point(8.579, -8.993, 35.928),
                             Point(7.526, -7.293, 34.653),
                             Point(6.38, -5.654, 33.33),
                             Point(5.111, -4.11, 31.951),
                             Point(3.699, -2.695, 30.511),
                             Point(2.145, -1.438, 29),
                             Point(0.239, -0.239, 0),
                             Point(0.239, -0.239, 0),
                             Point(-16.691, 10.409, 0)});

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

    // cout << "Robot X: " << chassis.get_X_position() << endl;
    // cout << "Robot Y: " << chassis.get_Y_position() << endl;
    // cout << "======================================" << endl;
    // cout << "Closest Point X: " << closestPoint.x << endl;
    // cout << "Closest Point Y: " << closestPoint.y << endl;
    // cout << "======================================" << endl;

    // Get the point where the circle intersects, it doesn't have to be a point that's in the path, just whatever x, y position it intersections the line
    lookAheadPoint = this->findLookAheadPoint(path, lastLookAheadPoint, lastLookAheadPointIndex, closestPointIndex, lookAheadDistance, chassis.get_X_position(), chassis.get_Y_position());

    if (lookAheadPoint == lastLookAheadPoint)
    {
      // cout << "LAX: " << lookAheadPoint.x << "\n";
      // cout << "LAY: " << lookAheadPoint.y << "\n";
      // test(lookAheadPoint.x, lookAheadPoint.y, kP, kI, kD);
      // wait(waitTime, vex::timeUnits::msec);
      chassis.drive_to_point(lookAheadPoint.x, lookAheadPoint.y, 3, 3);
      continue;
    }

    // cout << "RX: " << chassis.get_X_position() << endl;
    // cout << "RY: " << chassis.get_Y_position() << endl;
    // cout << "======================================" << endl;
    // cout << "LPX: " << lookAheadPoint.x << endl;
    // cout << "LPY: " << lookAheadPoint.y << endl;
    // cout << "======================================" << endl;

    // cout << "Gyro: " << reduce_negative_180_to_180(chassis.Gyro.rotation()) << endl;
    // cout << "Absolute: " << chassis.get_absolute_heading() << endl;

    // Get the curvature of the path
    curvature = this->getCurvature(M_PI / 2 - to_rad(reduce_negative_180_to_180(chassis.Gyro.rotation())), chassis.get_X_position(), chassis.get_Y_position(), lookAheadPoint.x, lookAheadPoint.y);

    // Calculate the velocities
    targetVelocity = closestPoint.speed;
    targetVelocity = slew(targetVelocity, previousTargetVelocity, 0.5);
    previousTargetVelocity = targetVelocity;

    // cout << "Curvature: " << curvature << endl;

    // Calculate the target speeds for the motors
    leftTargetVelocity = (targetVelocity * (2 + curvature * this->trackWidth) / 2);
    rightTargetVelocity = (targetVelocity * (2 - curvature * this->trackWidth) / 2);

    // cout << "LV: " << leftTargetVelocity << "\n";
    // cout << "RV:  " << rightTargetVelocity << "\n";
    // cout << "=============================================\n";

    double ratio = max(leftTargetVelocity, rightTargetVelocity) / 127;
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

      // test(lookAheadPoint.x, lookAheadPoint.y, kP, kI, kD);
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
}