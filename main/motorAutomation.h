//PID controllers
#define PTagSpeed 10
#define ITagSpeed 10
#define PTagPos 1
#define DTagPos 1

//position control accuracy
#define accuracy 1

//constants for recursive filter
#define wholePart 100000
#define newPart 10000 //max 1000
const int oldPart = wholePart - newPart;

//maximum acceleration (speed change in a cycle)
#define maxAccelC 0.005
const int maxAccel = myinterval * maxAccelC;

//Tárolók
//időmérő
//elapsedMicros elapsedTime;
//szűrt sebesség hiba
volatile int aggrSpeedLeft = 0;
volatile int aggrSpeedRight = 0;
//szűrt pozívió hiba derivált
volatile int errDiffAggrA = 0;
volatile int errDiffAggrB = 0;
//régi hiba
volatile int errOldLeft = 0;
volatile int errOldRight = 0;
//integráló tag
volatile long long errSumLeft = 0;
volatile long long errSumRight = 0;
//régi setspeed
volatile int pastSetSpeedLeft = 0;
volatile int pastSetSpeedRight = 0;

#include "motorControl.h"
#include "encoderReader.h"

//Speed control PI
void SetMotorSpeed(double setSpeedLeft, double setSpeedRight)
{
  //Maximum gyorsulás beállítása
  if (abs(setSpeedLeft - pastSetSpeedLeft) > maxAccel) setSpeedLeft = pastSetSpeedLeft + sign(setSpeedLeft - pastSetSpeedLeft) * maxAccel;
  if (abs(setSpeedRight - pastSetSpeedRight) > maxAccel) setSpeedRight = pastSetSpeedRight + sign(setSpeedRight - pastSetSpeedRight) * maxAccel;
  //Encoder érték kiolvasás
  int leftPos = encoderLeft.read();
  int rightPos = encoderRight.read();
  //Sebesség számítása + rekurzív szűrés sebességre
  aggrSpeedLeft = (oldPart * aggrSpeedLeft + newPart * 1000 * (leftPos - leftPosOld)) / wholePart;
  aggrSpeedRight = (oldPart * aggrSpeedRight + newPart * 1000 * (rightPos - rightPosOld)) / wholePart;

//  //0 közeli hiba kiiktatása
//  if (abs(aggrSpeedLeft) < 3)
//  {
//    aggrSpeedLeft = 0;
//  }
//  if (abs(aggrSpeedRight) < 3)
//  {
//    aggrSpeedRight = 0;
//  }

  //Sebesség hiba számítása
  int errorA = setSpeedLeft - aggrSpeedLeft;
  int errorB = setSpeedRight - aggrSpeedRight;

  //Tárolóba pozíció mentés
  leftPosOld = leftPos;
  rightPosOld = rightPos;

  //Integráló tag növelése
  errSumLeft += errorA;
  errSumRight += errorB;

  //Tárolóba setSpeet metése
  pastSetSpeedLeft = setSpeedLeft;
  pastSetSpeedRight = setSpeedRight;

  //double opVA = sA + PtagSpeed * errorA + Itag * errSumLeft;
  //double opVB = sB + PtagSpeed * errorB + Itag * errSumRight;
  int opVA = PTagSpeed * errorA + ITagSpeed * errSumLeft;
  int opVB = PTagSpeed * errorB + ITagSpeed * errSumRight;

  SetMotorPower(opVA, opVB);
}

//Position control (incremental) PD
void SetPos(double spA, double spB)
{
  int leftPos = encoderLeft.read();
  int rightPos = encoderRight.read();
  double errorA = spA - leftPos;
  double errorB = spB - rightPos;
  if (abs(errorA) < accuracy)
  {
    errorA = 0;
    errSumLeft = 0;
    errDiffAggrA = 0;
  }
  if (abs(errorB) < accuracy)
  {
    errorB = 0;
    errSumRight = 0;
    errDiffAggrB = 0;
  }
  errDiffAggrA = 0.9 * errDiffAggrA + 0.1 * (errorA - errOldLeft);
  errDiffAggrB = 0.9 * errDiffAggrB + 0.1 * (errorB - errOldRight);
  errSumLeft = 0.9 * errSumLeft + errorA;
  errSumRight = 0.9 * errSumRight + errorB;

  double opVA = PTagPos * errorA + DTagPos * errDiffAggrA;
  double opVB = PTagPos * errorB + DTagPos * errDiffAggrB;

  errOldLeft = errorA;
  errOldRight = errorB;

  SetMotorSpeed(opVA, opVB);
}


//Call this to setup drive system
void SetupMotorAutomation() {
  SetupMotors();
}

