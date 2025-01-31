#include <iostream>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <fstream>
#include <string>
#include <limits.h>
#include "functional"

using namespace std;

struct Point
{
  double x;
  double y;
  double speed;
};

typedef vector<Point> Path;

int main()
{
  ifstream pathFile("path.txt");
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

  for (auto Point : path)
    cout << "Point(" << Point.x << ", " << Point.y << ", " << Point.speed << ")," << endl;

  return 0;
}