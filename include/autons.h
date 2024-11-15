#pragma once
#include "JAR-Template/drive.h"

class Drive;

extern Drive chassis;

void default_constants();
int intakeToFishyTask();
void scoreFishy();

void alliance_negative(std::string c);
void basic_negative(std::string c);
void doinker_positive(std::string c);
void alliance_positive(std::string c);
void basic_positive(std::string c);

void alliance_negative_blue();
void basic_negative_blue();
void doinker_positive_blue();
void alliance_positive_blue();
void basic_positive_blue();
void alliance_negative_red();
void basic_negative_red();
void doinker_positive_red();
void alliance_positive_red();
void basic_positive_red();
void auton_skills();