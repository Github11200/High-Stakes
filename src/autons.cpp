#include "vex.h"
#include "../include/driver/intake.h"
#include "../include/driver/ladyBrown.h"
#include "../include/driver/mogo.h"
#include "../include/auto/skills.h"
#include "../include/auto/negativeRingRush.h"
#include "../include/auto/positiveGoalRush.h"

using namespace vex;
using namespace std;

bool intakeToLadyBrownAuton = false;
bool intakeSort = false;
bool intakeRev = false;
bool intakeRevSlow = false;
bool ladyBrownScore = false;
bool ladyBrownAllianceStakeScore = false;
bool ladyBrownAllianceHoldUp = false;
bool keepAllianceRing = false;
float ladyBrownDelay = 0;
bool mogoClamp = false;
float mogoClampDelay = 0;
bool intakeToGhostFrong = false;
bool intakeSpin = false;
bool shouldAntiJam = true;
IntakeControl intakeControl(12);
LadyBrown negativeRingRushLadyBrown(28, 140, 190, 116, 12, 0.3, 1.5, 0.5, 10, 1200, 12, 360, 2);
LadyBrown soloAwpLadyBrown(116, 166, 210, 116, 9, 0.4, 1.5, 0.5, 10, 1200, 12, 360, 2);
MogoControl mogoControl;

vector<Point> Autonomous::mirrorPath(vector<Point> originalPath)
{
  for (int i = 0; i < originalPath.size(); ++i)
    originalPath[i].x = -originalPath[i].x;
  return originalPath;
}

DriveParams goal_constants()
{
  DriveParams goalParams;
  goalParams.set_max_voltage(12).set_kp(0.7).set_ki(0.01).set_kd(3.5).set_starti(3);
  goalParams.set_heading_max_voltage(10).set_heading_kp(0.15).set_heading_ki(0.02).set_heading_kd(1).set_heading_starti(1);

  return goalParams;
}

ExitConditions fast_exit_conditions()
{
  ExitConditions fastExitConditions;
  fastExitConditions.driveParams.set_settle_error(2).set_settle_time(0.01).set_timeout(4000);
  fastExitConditions.turnParams.set_settle_error(2).set_settle_time(0.01).set_timeout(800);
  fastExitConditions.swingParams.set_settle_error(1).set_settle_time(5000).set_timeout(5000);

  return fastExitConditions;
}

ExitConditions exact_exit_conditions()
{
  ExitConditions exactExitConditions;
  exactExitConditions.driveParams.set_settle_error(0.5).set_settle_time(200).set_timeout(4000);
  exactExitConditions.turnParams.set_settle_error(1).set_settle_time(200).set_timeout(1200);
  exactExitConditions.swingParams.set_settle_error(1).set_settle_time(5000).set_timeout(5000);

  return exactExitConditions;
}

Autonomous::Autonomous()
{
  this->allianceColor = allianceColor;

  static IntakeControl *intakeControlThingy = &intakeControl;
  static LadyBrown *ladyBrownThingy = &negativeRingRushLadyBrown;
  static MogoControl *mogoControlThingy = &mogoControl;
  this->intakeAutonTask = thread([]()
                                 { while (true) {
                                    intakeControlThingy->intakeAutonTask();
                                    wait(50, vex::timeUnits::msec);
                                  } });
  // this->ladyBrownAutonTask = thread([]()
  //                                   { while (true) {
  //                                       ladyBrownThingy->ladyBrownAutonTask();
  //                                       wait(50, vex::timeUnits::msec);
  //                                     } });
  this->mogoAutonTask = thread([]()
                               {
                                  while (true) {
                                    mogoControlThingy->mogoAutonTask(); 
                                    wait(50, vex::timeUnits::msec);
                                  } });

  this->driveParamsWithMogo.set_kp(driveParams.drive_kp + 0.05);
  this->driveParamsWithMogo.set_kd(driveParams.drive_kd + 0.5);
  this->driveParamsWithMogo.set_drive_slew(driveParams.drive_slew - 0.65);

  this->turnParamsWithMogo.set_kp(turnParams.turn_kp - 0.017);
}

Autonomous::~Autonomous()
{
  this->intakeAutonTask.interrupt();
  this->intakeAutonTask.~thread();

  this->ladyBrownAutonTask.interrupt();
  this->ladyBrownAutonTask.~thread();

  cout << "Killed it" << endl;
}

void Autonomous::setAllianceColor(vex::color allianceColor) { this->allianceColor = allianceColor; }

// TESTED, HAHA
void Autonomous::solo_awp()
{
  int reversed;

  LadyBrownRotation.setPosition(29.4, vex::rotationUnits::deg);

  if (this->allianceColor == vex::color::red)
  {
    reversed = 1;
    alliance = "red";
    chassis.set_coordinates(-57.00, 7.6944, 225);
  }
  else
  {
    reversed = -1;
    alliance = "blue";
    // FIX BLUE COORDINATES
    chassis.set_coordinates(57.00, 7.6944, 135);
  }

  static LadyBrown ladyBrownPointerThing = soloAwpLadyBrown;
  static Drive driveReference = chassis;

  cout << "solo awp auto started, initial position:" << endl;
  cout << chassis.get_X_position() << ", " << chassis.get_Y_position() << endl;

  // Score on alliance stake
  thread allianceStakeScore = thread([]()
                                     {  soloAwpLadyBrown.allianceStakeScore();    
                                        this_thread::yield(); });
  chassis.drive_distance(7, chassis.get_absolute_heading(), DriveParams().set_settle_time(0).set_timeout(400));

  cout << "solo awp auto started, initial position:" << endl;
  cout << chassis.get_X_position() << ", " << chassis.get_Y_position() << endl;

  // thread odomPrinting = thread([]()
  //                              {
  //                                while(true)
  //                                {
  //                                 //  Brain.Screen.clearScreen();
  //                                 //  Brain.Screen.printAt(0, 20, "X: %.5f", chassis.get_X_position());
  //                                 //  Brain.Screen.printAt(0, 60, "Y: %.5f", chassis.get_Y_position());
  //                                 //  Brain.Screen.printAt(0, 100, "Theta: %.5f", chassis.get_absolute_heading());
  //                                 printf("Theta: %.5f\n", chassis.get_absolute_heading());
  //                                  wait(20, vex::timeUnits::msec);
  //                                } });

  // Drive in front of the mogo
  chassis.drive_to_point(-38.3886 * reversed, 27.6128, DriveParams().set_timeout(1200).set_settle_time(0).set_settle_error(2.3).set_min_voltage(2));

  // Pull the lady brown back
  thread autonLoadingThread = thread([]()
                                     { soloAwpLadyBrown.autonLoading(); cout << "brought it back..." << endl; this_thread::yield(); });

  // Turn towards the mogo
  chassis.turn_to_point(-20.099 * reversed, 16.6367, 180, TurnParams().set_timeout(800).set_settle_error(2).set_settle_time(10));

  // Move into goal and clamp
  mogoClampDelay = 800;
  mogoClamp = true;

  // Drive into the mogo and clamp
  chassis.drive_to_point(-20.099 * reversed, 16.6367, DriveParams().set_timeout(1500).set_settle_time(0).set_settle_error(2));

  intakeSort = true;
  DriveParams temporaryDriveWithMogoParams = this->driveParamsWithMogo;

  // Get the first ring in the 8 stack
  chassis.turn_to_point(-8.35572 * reversed, 36.9112, 0, TurnParams().set_settle_time(0).set_settle_error(2));
  chassis.drive_to_point(-8.35572 * reversed, 36.9112, temporaryDriveWithMogoParams.set_settle_time(0).set_settle_error(2));

  temporaryDriveWithMogoParams = this->driveParamsWithMogo;
  // Turn and get the second ring in the 8 stack
  chassis.turn_to_point(-8.35572 * reversed, 55.2747, 0, TurnParams().set_settle_time(0).set_settle_error(1));
  chassis.drive_to_point(-8.35572 * reversed, 55.2747, temporaryDriveWithMogoParams.set_settle_time(0).set_settle_error(5).set_min_voltage(8));
  wait(800, vex::timeUnits::msec);

  // chassis.drive_to_pose(-12.7989, 52.2747, 0, 0.9, 0, temporaryDriveWithMogoParams.set_max_voltage(5));

  temporaryDriveWithMogoParams = this->driveParamsWithMogo;

  // Drive back from the stack
  chassis.drive_to_pose(-18.462 * reversed, 23.588, (allianceColor == vex::color::red ? 210 : (360 - 210)), 0.1, 0, temporaryDriveWithMogoParams.set_max_voltage(7).set_timeout(900));

  // Turn towards the **first** 2 stack of rings (alliance color on bottom) and intake it
  chassis.turn_to_point(-23.2508 * reversed, 45.0579, 0, TurnParams().set_settle_time(0).set_settle_error(3).set_timeout(800));
  temporaryDriveWithMogoParams = this->driveParamsWithMogo;
  chassis.drive_to_point(-23.2508 * reversed, 45.0579, temporaryDriveWithMogoParams.set_settle_time(0).set_settle_error(3).set_timeout(900));

  // Turn towards the other 2 stack (with the rings flipped) and drive into it
  chassis.turn_to_point(-60.9473 * reversed, -29.3568, 0, TurnParams().set_settle_time(100).set_settle_error(2).set_timeout(1200));
  Clamp.set(false);
  mogoClamp = false;
  intakeSort = false;
  keepAllianceRing = true;

  // goon skibidi
  chassis.drive_to_point(-60.9473 * reversed, -29.3568, DriveParams().set_timeout(600).set_max_voltage(12).set_settle_time(0).set_settle_error(4));
  chassis.drive_to_point(-60.9473 * reversed, -29.3568, DriveParams().set_timeout(3000).set_max_voltage(5).set_settle_time(0).set_settle_error(4));

  while (keepAllianceRing)
    wait(20, vex::timeUnits::msec);

  // Turn towards the second mogo and drive into it
  chassis.turn_to_point(-16.2287 * reversed, -25.8152, 180, TurnParams().set_timeout(800).set_settle_time(0).set_settle_error(2));
  intakeSort = false;
  mogoClampDelay = 800;
  mogoClamp = true;
  chassis.drive_to_point(-22.2287 * reversed, -25.8152, DriveParams().set_timeout(1500).set_settle_time(0).set_settle_error(1).set_drive_slew(12));
  keepAllianceRing = false;
  intakeToGhostFrong = false;
  ringStopped = false;
  intakeRev = false;
  intakeToLadyBrownAuton = false;

  intakeSort = true;
  autonLoadingThread.interrupt();
  autonLoadingThread.~thread();
  allianceStakeScore.interrupt();
  allianceStakeScore.~thread();
  chassis.turn_to_angle((this->allianceColor == vex::color::red ? 20 : 340), this->turnParams.set_timeout(1500).set_settle_time(0).set_settle_error(2));
  thread spinny = thread([]()
                         { LadyBrownMotor.spin(vex::directionType::fwd, 12, vex::voltageUnits::volt); });
  chassis.drive_distance(5, chassis.get_absolute_heading(), DriveParams());
  spinny.interrupt();
  spinny.~thread();
  wait(100, vex::timeUnits::sec);
}

// NOT TESTED
void Autonomous::positive_six_ring()
{
  int reversed;

  if (this->allianceColor == vex::color::red)
  {
    chassis.set_coordinates(-51.312, -29.859, 78);
    reversed = -1;
    alliance = "red";
  }
  else
  {
    chassis.set_coordinates(51.312, -29.859, 282);
    reversed = 1;
    alliance = "blue";
  }

  cout << "positive six ring started, initial position:" << endl;
  cout << chassis.get_X_position() << ", " << chassis.get_Y_position() << ", " << chassis.get_absolute_heading() << endl;

  // Drive back and clamp the goal
  mogoClampDelay = 800;
  mogoClamp = true;
  chassis.drive_to_point(23.701, -23.701);
  intakeSort = true;

  // Go to the center and doinker two rings
  chassis.turn_to_point(9.995, -10.79, 0);
  chassis.drive_to_point(9.995, -10.79);
  LeftDoinker.set(true);
  chassis.turn_to_angle(300);
  chassis.drive_distance(5, 300);
  RightDoinker.set(true);

  // Go back, lining up the doinkered rings in a smooth path for later
  chassis.drive_to_pose(40.784, -35.421, 330, 0.5, 0);
  LeftDoinker.set(false);
  RightDoinker.set(false);

  // Curve to eat all the doinkered rings + the one in a stack
  chassis.turn_to_angle(0);
  chassis.drive_distance(10, 0);
  chassis.right_swing_to_angle(180);
  chassis.drive_to_point(23.493, -47.191);

  // Eat two rings from the corner
  chassis.turn_to_angle(90);
  chassis.drive_to_pose(62.507, -62.507, 135, 0.5, 0);
  vex::wait(300, msec);
  chassis.drive_distance(-15, 135);
  RightDoinker.set(true);
  chassis.drive_distance(10, 135);
  vex::wait(300, msec);

  // Clear out the corner and drop goal in
  chassis.turn_to_angle(0);
  RightDoinker.set(false);
  chassis.turn_to_point(66.544, -66.544, 180);
  Clamp.set(false);

  // Touch ladder
  chassis.drive_to_pose(11.771, -32.102, 303, 0.5, 0);
  ladyBrownAllianceStakeScore = true;
  vex::wait(100, sec);
}

// TESTED, WORKS WELL IG
void Autonomous::negative_ring_rush()
{
  int reversed;

  if (this->allianceColor == vex::color::red)
  {
    reversed = 1;
    alliance = "red";
    chassis.set_coordinates(-51.582, 28.739, 71);
  }
  else
  {
    reversed = -1;
    alliance = "blue";
    chassis.set_coordinates(51.582, 28.739, 289);
  }

  cout << "negative ring rush auto started, initial position:" << endl;
  cout << chassis.get_X_position() << ", " << chassis.get_Y_position() << endl;

  // static LadyBrown ladyBrownPointerThing = ladyBrown;
  // thread allianceStakeScore = thread([]()
  //                                    {  ladyBrown.score();
  //                                       this_thread::yield(); });

  keepAllianceRing = true;
  DriveParams temporaryDriveWithMogoParams = this->driveParamsWithMogo;

  // Get the 8 stack of rings
  if (this->allianceColor == vex::color::red)
    LeftDoinker.set(true);
  else
    RightDoinker.set(true);
  keepAllianceRing = true;
  // This was 41.9829 for red, changing it higher for blue. Dunno if red should stay the same.
  // chassis.drive_to_point(-9.75801 * reversed, 42.9829, DriveParams().set_settle_time(0).set_settle_error(2).set_drive_slew(0.6).set_timeout(1100));
  if (this->allianceColor == vex::color::red)
    chassis.drive_to_point(-9.75801 * reversed, 41.9829, DriveParams().set_settle_time(0).set_settle_error(2).set_drive_slew(0.6).set_timeout(1100));
  else
    chassis.drive_to_point(-9.75801 * reversed, 44.9829, DriveParams().set_settle_time(0).set_settle_error(2).set_drive_slew(0.6).set_timeout(1100));
  keepAllianceRing = true;

  // Drive and clamp into the goal
  chassis.turn_to_point(-22.8325 * reversed, 20, 180, TurnParams().set_settle_time(0).set_settle_error(2));
  mogoClampDelay = 1000;
  mogoClamp = true;
  chassis.drive_to_point(-22.8325 * reversed, 20, DriveParams().set_drive_slew(12).set_settle_time(0).set_settle_error(1.5).set_timeout(1100).set_drive_slew(12));
  wait(200, vex::timeUnits::msec);

  // Turn, let go of the ring, and then intake both of them
  keepAllianceRing = false;
  intakeSort = true;
  shouldAntiJam = true;
  chassis.turn_to_point(-20.8112 * reversed, 52.399, 0, TurnParams().set_settle_time(0).set_settle_error(2));
  LeftDoinker.set(false);
  RightDoinker.set(false);
  temporaryDriveWithMogoParams = this->driveParamsWithMogo;
  chassis.drive_to_point(-20.8112 * reversed, 52.399, temporaryDriveWithMogoParams.set_settle_time(0).set_settle_error(3).set_timeout(1100));

  // Line up in front of the corner
  chassis.turn_to_point(-56 * reversed, 54, 0, TurnParams().set_settle_time(0).set_settle_error(2));
  chassis.drive_to_point(-56 * reversed, 54, temporaryDriveWithMogoParams.set_settle_time(0).set_settle_error(3));

  // Turn and drive into the corner
  chassis.turn_to_point(-67.5711 * reversed, 65.7478, 0, TurnParams().set_settle_time(0).set_settle_error(2));
  chassis.drive_to_point(-67.5711 * reversed, 65.7478, temporaryDriveWithMogoParams.set_max_voltage(4).set_timeout(1200));
  // chassis.drive_distance(100000, chassis.get_absolute_heading(), DriveParams().set_timeout(300).set_drive_slew(12));
  wait(200, vex::timeUnits::msec);

  // Move back from the corner
  temporaryDriveWithMogoParams = this->driveParamsWithMogo;
  chassis.drive_distance(-25, chassis.get_absolute_heading(), DriveParams().set_min_voltage(5).set_drive_slew(12).set_timeout(1500));
  chassis.drive_distance(21, chassis.get_absolute_heading(), DriveParams().set_timeout(1500));

  bool quals = true;
  if (quals)
  {
    // Turn and intake the preload to lady brown
    temporaryDriveWithMogoParams = this->driveParamsWithMogo;
    chassis.turn_to_point(-54.6043 * reversed, 15.4858, 0, TurnParams().set_settle_time(0).set_settle_error(2));
    // thread ladyBrownLoading = thread([]()
    //                                  { negativeRingRushLadyBrown.autonLoading(); });
    // shouldAntiJam = false;
    chassis.drive_to_point(-54.6043 * reversed, 15.4858, temporaryDriveWithMogoParams.set_settle_time(0).set_settle_error(3).set_timeout(1000));

    // Turn towards and ladder and touch it all over
    temporaryDriveWithMogoParams = this->driveParamsWithMogo;
    chassis.turn_to_point(-33.9902 * reversed, 2.93012, 0, TurnParams().set_settle_time(0).set_settle_error(2));
    thread touchyTheLadder = thread([]()
                                    { wait(500, vex::timeUnits::msec);
                                    LadyBrownMotor.spin(vex::directionType::fwd, 12, vex::voltageUnits::volt); });
    chassis.drive_distance(27, chassis.get_absolute_heading(), DriveParams());
  }
  else
  { // Turn and intake the preload to lady brown
    temporaryDriveWithMogoParams = this->driveParamsWithMogo;
    return;
    chassis.turn_to_point(-54.6043 * reversed, 16.8874, 0, TurnParams().set_settle_time(0).set_settle_error(2));
    thread ladyBrownLoading = thread([]()
                                     { negativeRingRushLadyBrown.autonLoading(); });
    shouldAntiJam = false;
    chassis.drive_to_point(-54.6043 * reversed, 16.8874, temporaryDriveWithMogoParams.set_settle_time(0).set_settle_error(2).set_timeout(1200));

    if (this->allianceColor == vex::color::blue)
      RightDoinker.set(true);
    else
      LeftDoinker.set(true);
    wait(1000, vex::timeUnits::msec);

    // Move the ring down
    chassis.turn_to_angle((this->allianceColor == vex::color::blue ? 128.277 : (360 - 128.277)));
    LeftDoinker.set(false);
    RightDoinker.set(false);
    wait(1000, vex::timeUnits::msec);

    // Turn towards the alliance stake and score
    chassis.turn_to_point(-61.7795 * reversed, 12.3834, 0, TurnParams());
    ladyBrownLoading.interrupt();
    ladyBrownLoading.~thread();
    negativeRingRushLadyBrown.resetLadyBrownPID();
    thread ladyBrownAllianceScore = thread([]()
                                           { negativeRingRushLadyBrown.allianceStakeScore(); });
    chassis.drive_to_point(-61.7795 * reversed, 12.3834, DriveParams());

    // chassis.turn_to_point(-56.3545, 17.3865);
    // chassis.drive_to_point(-56.3545, 17.3865);
    // chassis.turn_to_angle(233.417);
    // chassis.turn_to_point(-63.4791, 14.0967);
    // chassis.drive_to_point(-63.4791, 14.0967);
  }
}

//  NOT TESTED
void Autonomous::positive_baker()
{
  cout << "positive goal rush auto started, initial position:" << endl;
  cout << chassis.get_X_position() << ", " << chassis.get_Y_position() << endl;

  int reversed;
  if (this->allianceColor == vex::color::red)
  {
    reversed = 1;
    chassis.set_coordinates(-51.704, -23.582, 270);
  }
  else
  {
    reversed = -1;
    chassis.set_coordinates(51.704, -23.582, 90);
  }

  DriveParams tempoaryDriveParamsWithMogo = this->driveParamsWithMogo;

  // Move back and clamp onto the goal
  mogoClamp = true;
  mogoClampDelay = 900;
  chassis.drive_to_point(-19.294 * reversed, -23.582, DriveParams().set_settle_time(0).set_settle_error(3));

  // Turn towards the square of rings in the middle, drive there, and get the first ring
  chassis.turn_to_point(-10.467 * reversed, -11.35, 0, TurnParams().set_settle_time(0).set_settle_error(2).set_timeout(900));
  intakeSort = true;
  chassis.drive_to_point(-10.467 * reversed, -11.35, tempoaryDriveParamsWithMogo.set_settle_time(0).set_settle_error(2));
  if (this->allianceColor == vex::color::red)
    RightDoinker.set(true);
  else
    LeftDoinker.set(true);
  wait(100, vex::timeUnits::msec);

  // Turn a little then get the second ring
  chassis.turn_to_angle((this->allianceColor == vex::color::red ? 60 : (360 - 60)), TurnParams().set_timeout(500));
  if (this->allianceColor == vex::color::red)
    LeftDoinker.set(true);
  else
    RightDoinker.set(true);
  wait(100, vex::timeUnits::msec);

  // Drive back with the rings
  tempoaryDriveParamsWithMogo = this->driveParamsWithMogo;
  chassis.turn_to_point(-49.182 * reversed, -46.66, 180, TurnParams().set_settle_time(0).set_settle_error(1).set_timeout(500));
  chassis.drive_to_point(-49.182 * reversed, -46.66, tempoaryDriveParamsWithMogo.set_settle_time(0).set_settle_error(3));

  // Lift up the doinkers
  LeftDoinker.set(false);
  RightDoinker.set(false);
  wait(100, vex::timeUnits::msec);

  // Turn towards the first ring and intake it
  tempoaryDriveParamsWithMogo = this->driveParamsWithMogo;
  chassis.turn_to_point(-37.454 * reversed, -25.726, 0, TurnParams().set_settle_time(0).set_settle_error(1).set_timeout(800));
  chassis.drive_to_point(-37.454 * reversed, -25.726, tempoaryDriveParamsWithMogo.set_settle_time(0).set_settle_error(2));

  // Turn and and then intake the other two rings
  tempoaryDriveParamsWithMogo = this->driveParamsWithMogo;
  chassis.turn_to_point(-21.943 * reversed, -49.686, 0, TurnParams().set_settle_time(0).set_settle_error(2).set_timeout(900));
  chassis.drive_to_point(-21.943 * reversed, -49.686, tempoaryDriveParamsWithMogo.set_settle_time(0).set_settle_error(3));

  // Drive in front of the corner
  tempoaryDriveParamsWithMogo = this->driveParamsWithMogo;
  chassis.turn_to_point(-51.704 * reversed, -49.686, 0, TurnParams().set_settle_time(0).set_settle_error(2).set_timeout(800));
  chassis.drive_to_point(-51.704 * reversed, -49.686, tempoaryDriveParamsWithMogo.set_settle_time(0).set_settle_error(2));

  // Go into the corner and intake 2 rings by moving back and forward
  tempoaryDriveParamsWithMogo = this->driveParamsWithMogo;
  chassis.turn_to_point(-66.332 * reversed, -65.828, 0, TurnParams().set_settle_time(0).set_settle_error(3).set_timeout(800));
  chassis.drive_to_point(-66.332 * reversed, -65.828, tempoaryDriveParamsWithMogo.set_timeout(500));
  chassis.drive_distance(-25, chassis.get_absolute_heading(), DriveParams().set_min_voltage(5).set_drive_slew(12).set_timeout(1500));
  chassis.drive_distance(21, chassis.get_absolute_heading(), DriveParams().set_timeout(1500));

  // Move back to the middle after dropping the goal off in the corner
  tempoaryDriveParamsWithMogo = this->driveParamsWithMogo;
  chassis.turn_to_point(-11.224 * reversed, -47.669, 0, TurnParams().set_settle_time(0).set_settle_error(3));
  Clamp.set(false);
  wait(100, vex::timeUnits::msec);
  chassis.drive_to_point(-11.224 * reversed, -47.669, tempoaryDriveParamsWithMogo.set_settle_time(0).set_settle_error(3));
  chassis.turn_to_angle((this->allianceColor == vex::color::red ? 270 : 90), TurnParams().set_settle_time(0).set_settle_error(2));

  // And you're done, please remember to like, subscribe and share this auto!
}

void Autonomous::auton_skills()
{
  intakeToLadyBrownAuton = false;

  chassis.set_coordinates(-61, 0, 90);

  cout << "skills auto started, initial position:" << endl;
  cout << chassis.get_X_position() << ", " << chassis.get_Y_position() << endl;

  alliance = "red";

  Pursuit *purePursuit = new Pursuit(11.375);

  // Score on first alliance stake
  intakeSort = true;
  vex::wait(0.3, vex::timeUnits::sec);
  intakeSort = false;
  Intake.stop();

  // Back away
  chassis.drive_to_point(-47.212, 0, DriveParams().set_timeout(900));
  chassis.turn_to_point(-47.212, -23.503, 180, TurnParams());

  // Clamp onto the goal
  chassis.drive_to_point(-47.212, -23.503, DriveParams().set_max_voltage(6).set_timeout(900));
  Clamp.set(true);
  vex::wait(100, msec);
  intakeSort = true;

  // Eat a couple of rings, end up facing pile of blue
  purePursuit->followPath(skills[0], 12.75, 10000, true, 17, 3, 0.8);

  // Grab two red rings from under blue rings, holding one for frog
  chassis.turn_to_point(58.962, -47.269, 0, TurnParams().set_settle_error(2).set_timeout(800));
  chassis.drive_to_point(58.962, -47.269, DriveParams().set_timeout(800));
  intakeSort = false;
  intakeToLadyBrownAuton = true;

  // Align to wall stake
  chassis.turn_to_point(0, -41.202, 180, TurnParams());
  chassis.drive_to_point(6, -41.202, DriveParams());

  // Turn to the actual wall stake position, then drive forward
  chassis.turn_to_point(0 + 5, -69.388, 0, TurnParams().set_settle_error(0.5));
  chassis.drive_distance(30, chassis.get_absolute_heading(), DriveParams());
  wait(100, vex::timeUnits::msec);
  cout << "X: " << chassis.get_X_position() << "\nY: " << chassis.get_Y_position() << endl;

  // Score twice on wall stake
  negativeRingRushLadyBrown.autonScore(0);
  vex::wait(1000, msec);
  negativeRingRushLadyBrown.autonLoading();
  vex::wait(1000, msec);

  // Clean up the rest of the rings in the corner
  chassis.drive_to_point(0, -47.269, DriveParams().set_timeout(800));
  chassis.turn_to_point(-100, -47.269, 0, TurnParams());
  intakeSort = true;
  purePursuit->followPath(skills[1], 12.75, 10000, true, 17, 3.5, 0.8);

  // Drop the goal in the corner
  intakeSort = false;
  vex::wait(100, msec);
  Clamp.set(false);
  chassis.drive_distance(-10, chassis.get_absolute_heading(), DriveParams().set_timeout(600));

  // Eat the ring in front of ladder
  chassis.turn_to_point(1.576, -46.908, 0, TurnParams().set_timeout(400));
  chassis.drive_to_point(1.576, -46.908, DriveParams());
  chassis.turn_to_point(23.764, -23.764, 0, TurnParams());
  chassis.drive_to_point(23.764, -23.764, DriveParams().set_timeout(1100));

  // Clamp the far goal
  chassis.turn_to_point(47.141, 0, 180, TurnParams());
  chassis.drive_to_point(47.141, 0, DriveParams().set_max_voltage(5));
  Clamp.set(true);
  vex::wait(200, msec);
  intakeSort = true;

  // Line up in front of the ladder
  chassis.drive_to_point(23.764, -23.764, DriveParams());
  intakeSort = false;

  // Go through the ladder
  Intake.stop();
  chassis.turn_to_point(-23.764, 23.764, 0, TurnParams());
  chassis.drive_to_point(-23.764, 23.764, DriveParams());

  // Eat up the two stored rings, and eat the corner of the corner rings
  intakeSort = true;
  chassis.drive_to_point(-44.1, 44.1, DriveParams());

  chassis.turn_to_point(-45.86, 56.571, 0, TurnParams());
  chassis.drive_to_point(-45.86, 56.571, DriveParams());

  // Put the goal in the corner
  chassis.turn_to_point(-54.7, 61, 180, TurnParams());
  intakeSort = false;
  Clamp.set(false);
  chassis.drive_distance(-18, chassis.get_absolute_heading(), DriveParams().set_timeout(600));

  // Clamp onto the final fillable goal
  chassis.drive_to_point(-47.141, 48, DriveParams());
  chassis.turn_to_point(-47.141, 23.574, 180, TurnParams());
  chassis.drive_to_point(-47.141, 23.574, DriveParams().set_max_voltage(6));
  Clamp.set(true);
  vex::wait(100, msec);
  intakeSort = true;

  // Pick up one ring for frog
  chassis.turn_to_point(-23.503 - 2, 46.931 + 3, 0, TurnParams());
  chassis.drive_to_point(-23.503 - 2, 46.931 + 3, DriveParams());
  intakeSort = true;

  // Align in front of wall stake
  chassis.turn_to_point(0.75, 41.202, 0, TurnParams());
  chassis.drive_to_point(0.75, 41.202, DriveParams());

  // Turn to the actual wall stake position, then drive forward
  chassis.turn_to_point(0.75, 69.388, 0, TurnParams().set_settle_error(0.5));
  chassis.drive_distance(26, chassis.get_absolute_heading(), DriveParams());

  // Eat two rings before the blue corner
  intakeSort = true;
  chassis.drive_to_point(0, 53.127, DriveParams().set_timeout(800));
  chassis.turn_to_point(23.381, 47.007, 0, TurnParams());
  chassis.drive_to_point(23.381, 47.007, DriveParams());

  chassis.turn_to_point(23.381, 23.481, 0, TurnParams());
  chassis.drive_to_point(23.381, 23.481, DriveParams());

  // First ring is the corner one in the triangle
  chassis.turn_to_point(44.1, 44.1, 0, TurnParams());
  chassis.drive_to_point(44.1, 44.1, DriveParams().set_timeout(1100));

  // Back up and eat a ring x2
  chassis.turn_to_point(45.86, 56.571, 0, TurnParams());
  chassis.drive_to_point(45.86, 56.571, DriveParams().set_timeout(800));
  chassis.turn_to_point(42.6, 42.6, 180, TurnParams().set_timeout(200));
  chassis.drive_to_point(42.6, 42.6, DriveParams());
  chassis.turn_to_point(56.571, 45.86, 0, TurnParams());
  RightDoinker.set(true);
  chassis.drive_to_point(56.571, 45.86, DriveParams());

  // Doinker out the corner
  chassis.turn_to_point(60.449, 56.51, 0, TurnParams());
  chassis.drive_distance(15, chassis.get_absolute_heading(), DriveParams().set_timeout(600));
  chassis.turn_to_angle(270, TurnParams().set_timeout(400));

  // Put last full mogo in corner
  chassis.turn_to_point(62.449, 58.51, 180, TurnParams());
  Clamp.set(false);
  RightDoinker.set(false);
  chassis.drive_distance(-16.5, chassis.get_absolute_heading(), DriveParams().set_timeout(400));

  // Get out of the corner
  chassis.drive_distance(8, chassis.get_absolute_heading(), DriveParams());

  // Clamp the final goal
  intakeSort = false;
  chassis.turn_to_point(59.149, 23.481, 180, TurnParams());
  chassis.drive_to_point(59.149, 23.481, DriveParams());
  Clamp.set(true);
  vex::wait(100, msec);

  // Doinker and path to the last corner
  chassis.turn_to_point(56.559, 18.069, 0, TurnParams());
  RightDoinker.set(true);
  purePursuit->followPath(skills[4], 20, 10000, true, 17, 4, 0.8);

  chassis.turn_to_angle(0, TurnParams());
  chassis.turn_to_point(70, -70, 180, TurnParams());
  RightDoinker.set(false);
  Clamp.set(false);
  chassis.drive_distance(-18, chassis.get_absolute_heading(), DriveParams().set_timeout(500));
  chassis.drive_distance(10, chassis.get_absolute_heading(), DriveParams());

  // // Push last goal into corner
  // chassis.drive_timeout = 4000;
  // chassis.drive_to_point(65.449, -55.733);
  // default_constants();

  // chassis.drive_distance(20);

  // cout << "Final X: " << chassis.get_X_position() << endl;
  // cout << "Final Y: " << chassis.get_Y_position() << endl;
}

DriveParams changeThingy(float drive_max_voltage, float drive_kp, float drive_ki, float drive_kd, float drive_starti)
{
  DriveParams driveParams;
  driveParams.set_max_voltage(drive_max_voltage).set_kp(drive_kp).set_ki(drive_ki).set_kd(drive_kd).set_starti(drive_starti);
  return driveParams;
}

void Autonomous::testing()
{
  chassis.set_coordinates(0, 0, 0);

  DriveParams driveParams;
  TurnParams turnParams;

  while (true)
  {
    if (Controller.ButtonA.pressing()) // Reset coordinates
      chassis.set_coordinates(0, 0, 0);
    else if (Controller.ButtonB.pressing()) // Go forwards
      chassis.turn_to_angle(180, turnParams);
    else if (Controller.ButtonX.pressing()) // Go backwards
      chassis.turn_to_angle(0, turnParams);
    else if (Controller.ButtonY.pressing())
    { // Print data
      cout << "kP: " << turnParams.turn_kp << ", kI: " << turnParams.turn_ki << ", kD: " << turnParams.turn_kd << ", settle error: " << turnParams.turn_settle_error << ", drive slew: " << driveParams.drive_slew << endl;
      cout << "X: " << chassis.get_X_position() << ", Y: " << chassis.get_Y_position() << ", Theta: " << chassis.get_absolute_heading() << endl;
    }
    // Change PID constants
    else if (Controller.ButtonUp.pressing())
    {
      driveParams.set_kp(driveParams.drive_kp + 0.1);
      cout << "kP: " << driveParams.drive_kp << endl;
      driveParams = changeThingy(12, driveParams.drive_kp, driveParams.drive_ki, driveParams.drive_kd, 0);
    }
    else if (Controller.ButtonDown.pressing())
    {
      driveParams.set_kp(driveParams.drive_kp - 0.1);
      cout << "kP: " << driveParams.drive_kp << endl;
      driveParams = changeThingy(12, driveParams.drive_kp, driveParams.drive_ki, driveParams.drive_kd, 0);
    }
    else if (Controller.ButtonRight.pressing())
    {
      driveParams.set_kd(driveParams.drive_kd + 0.1);
      cout << "kD: " << driveParams.drive_kd << endl;
      driveParams = changeThingy(12, driveParams.drive_kp, driveParams.drive_ki, driveParams.drive_kd, 0);
    }
    else if (Controller.ButtonLeft.pressing())
    {
      driveParams.set_kd(driveParams.drive_kd - 0.1);
      cout << "kD: " << driveParams.drive_kd << endl;
      driveParams = changeThingy(12, driveParams.drive_kp, driveParams.drive_ki, driveParams.drive_kd, 0);
    }
    else if (Controller.ButtonL1.pressing())
    {
      driveParams.set_ki(driveParams.drive_ki + 0.1);
      cout << "kI: " << driveParams.drive_ki << endl;
      driveParams = changeThingy(12, driveParams.drive_kp, driveParams.drive_ki, driveParams.drive_kd, 0);
    }
    else if (Controller.ButtonL2.pressing())
    {
      driveParams.set_ki(driveParams.drive_ki - 0.1);
      cout << "kI: " << driveParams.drive_ki << endl;
      driveParams = changeThingy(12, driveParams.drive_kp, driveParams.drive_ki, driveParams.drive_kd, 0);
    }
    vex::wait(100, vex::timeUnits::msec);
  }
  vex::wait(100, sec);
}