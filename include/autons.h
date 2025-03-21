#pragma once
#include "JAR-Template/drive.h"
#include "driver/mogo.h"

class Drive;

extern Drive chassis;

struct ExitConditions
{
  DriveParams driveParams;
  TurnParams turnParams;
  SwingParams swingParams;
};

vector<Point> mirrorPath(vector<Point> originalPath);

void negative_alliance_stake_rush(std::string c);
void positive_six_ring(std::string c);
void negative_ring_rush(std::string c);
void positive_goal_rush(std::string c);
void testing(std::string c);

void auton_skills();