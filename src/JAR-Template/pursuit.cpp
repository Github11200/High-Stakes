#include "pursuit.h"

using namespace vex;
using namespace std;

double Pursuit::distance(Point a, Point b)
{
  return sqrt(pow(a.x - b.x, 2) + pow(a.y - b.y, 2));
}

double Pursuit::Dot(Point a, Point b)
{
  return a.x * b.x + a.y * b.y;
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

  double t1 = 0;
  double t2 = 0;

  for (int i = 1; i < path.size(); ++i)
  {
    Point rayStart(path[i - 1].x, path[i - 1].y, path[i - 1].speed);
    Point rayEnd(path[i].x, path[i].y, path[i].speed);
    Point sphere(currentX, currentY, 0);

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

    if (t1 >= 0 && t1 <= 1)
    {
      // t1 is the intersection and it's closer than t2
    }

    if (t2 >= 0 && t2 <= 1)
    {
      // t1 doesn't intersect so we either start inside the sphere or completely past it
    }
  }
}