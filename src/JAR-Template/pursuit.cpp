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
  Path path = vector<Point>({Point(0.239, -0.239, 100),
                             Point(0.253, 1.761, 99.63),
                             Point(0.263, 3.761, 99.684),
                             Point(0.271, 5.761, 96.237),
                             Point(0.277, 7.761, 90.783),
                             Point(0.281, 9.761, 84.979),
                             Point(0.283, 11.761, 78.749),
                             Point(0.283, 13.761, 71.982),
                             Point(0.279, 15.761, 64.509),
                             Point(0.274, 17.761, 56.049),
                             Point(0.266, 19.761, 46.059),
                             Point(0.254, 21.761, 33.188),
                             Point(0.24, 23.761, 16.486),
                             Point(0.298, 25.754, 16.486),
                             Point(0.873, 27.662, 16.486),
                             Point(1.923, 29.357, 16.486),
                             Point(3.334, 30.768, 16.486),
                             Point(5.003, 31.865, 16.486),
                             Point(6.835, 32.661, 16.486),
                             Point(8.77, 33.156, 16.486),
                             Point(10.759, 33.346, 16.486),
                             Point(12.753, 33.229, 16.486),
                             Point(14.705, 32.805, 16.486),
                             Point(16.559, 32.063, 16.486),
                             Point(18.269, 31.031, 16.486),
                             Point(19.79, 29.736, 24.638),
                             Point(21.09, 28.219, 32.79),
                             Point(22.159, 26.53, 40.942),
                             Point(23.005, 24.72, 49.094),
                             Point(23.414, 22.784, 57.246),
                             Point(23.55, 20.789, 57.246),
                             Point(23.673, 18.793, 57.246),
                             Point(23.782, 16.796, 57.246),
                             Point(23.871, 14.798, 57.246),
                             Point(23.935, 12.799, 57.246),
                             Point(23.966, 10.799, 57.246),
                             Point(23.952, 8.799, 57.246),
                             Point(23.878, 6.801, 56.534),
                             Point(23.734, 4.806, 46.649),
                             Point(23.54, 2.815, 34.002),
                             Point(23.357, 0.824, 11.668),
                             Point(23.336, 0.558, 0),
                             Point(23.336, 0.558, 0)});

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
      wait(waitTime, vex::timeUnits::msec);
      // chassis.drive_to_point(lookAheadPoint.x, lookAheadPoint.y, 3, 3);
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
    targetVelocity = slew(targetVelocity, previousTargetVelocity, 5);
    previousTargetVelocity = targetVelocity;

    // cout << "Curvature: " << curvature << endl;

    // Calculate the target speeds for the motors
    leftTargetVelocity = (targetVelocity * (2 + curvature * this->trackWidth) / 2);
    rightTargetVelocity = (targetVelocity * (2 - curvature * this->trackWidth) / 2);

    // cout << "LV: " << leftTargetVelocity << "\n";
    // cout << "RV:  " << rightTargetVelocity << "\n";
    // cout << "=============================================\n";

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