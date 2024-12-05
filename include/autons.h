#pragma once
#include "JAR-Template/drive.h"

class Drive;

extern Drive chassis;

void default_constants();

void negative_alliance_stake_rush(std::string c);
void positive_alliance_stake_rush(std::string c);
void negative_ring_rush(std::string c);
void positive_mogo_rush(std::string c);

void auton_skills();