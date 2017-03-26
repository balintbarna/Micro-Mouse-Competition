//PID controllers
#define PTagSpeed 10
#define ITagSpeed 10
#define PTagPos 250
#define DTagPos 0

//position control accuracy
#define accuracy 1

//constants for recursive filter
#define wholePart 100000
#define newPart 10000 //max 1000
const int oldPart = wholePart - newPart;

//maximum acceleration (speed change in a cycle)
#define maxAccelC 0.002
const int maxAccel = myinterval * maxAccelC;

//Tárolók
//időmérő
//elapsedMicros elapsedTime;
//szűrt sebesség hiba
volatile int aggrSpeedLeft = 0;
volatile int aggrSpeedRight = 0;
//szűrt pozívió hiba derivált
volatile int errDiffAggrLeft = 0;
volatile int errDiffAggrRight = 0;
//régi hiba
volatile int errOldLeft = 0;
volatile int errOldRight = 0;
//integráló tag
volatile long long errSumLeft = 0;
volatile long long errSumRight = 0;
//régi setspeed
volatile int pastSetSpeedLeft = 0;
volatile int pastSetSpeedRight = 0;
//régi pozicio
volatile int leftPosOld = 0;
volatile int rightPosOld = 0;

#include "motorControl.h"
#include "encoderReader.h"

//Speed control PI
void SetMotorSpeed(double setSpeedLeft, double setSpeedRight)
{
  //Maximum gyorsulás beállítása
  //if (abs(setSpeedLeft - pastSetSpeedLeft) > maxAccel) setSpeedLeft = pastSetSpeedLeft + sign(setSpeedLeft - pastSetSpeedLeft) * maxAccel;
  //if (abs(setSpeedRight - pastSetSpeedRight) > maxAccel) setSpeedRight = pastSetSpeedRight + sign(setSpeedRight - pastSetSpeedRight) * maxAccel;
  //Encoder érték kiolvasás
  int leftPos = encoderLeft.read();
  int rightPos = encoderRight.read();
  //Sebesség számítása + rekurzív szűrés sebességre
  aggrSpeedLeft = (oldPart * aggrSpeedLeft + newPart * 1000 * (leftPos - leftPosOld)) / wholePart;
  aggrSpeedRight = (oldPart * aggrSpeedRight + newPart * 1000 * (rightPos - rightPosOld)) / wholePart;

  //  //0 közeli hiba kiiktatása
  //  if (abs(aggrSpeedLeft) < 30)
  //  {
  //    aggrSpeedLeft = 0;
  //  }
  //  if (abs(aggrSpeedRight) < 30)
  //  {
  //    aggrSpeedRight = 0;
  //  }

  //Sebesség hiba számítása
  int errorLeft = setSpeedLeft - aggrSpeedLeft;
  int errorRight = setSpeedRight - aggrSpeedRight;

  //Tárolóba pozíció mentés
  leftPosOld = leftPos;
  rightPosOld = rightPos;

  //Integráló tag növelése
  errSumLeft += errorLeft;
  errSumRight += errorRight;

  //Tárolóba setSpeet metése
  pastSetSpeedLeft = setSpeedLeft;
  pastSetSpeedRight = setSpeedRight;

  //PI control
  int opLeft = PTagSpeed * errorLeft + ITagSpeed * errSumLeft;
  int opRight = PTagSpeed * errorRight + ITagSpeed * errSumRight;

  //Send output
  SetMotorPower(opLeft, opRight);
}

//Position control (incremental) PD
void SetPos(int setPosLeft, int setPosRight)
{
  int leftPos = encoderLeft.read();
  int rightPos = encoderRight.read();
  int errorLeft = setPosLeft - leftPos;
  int errorRight = setPosRight - rightPos;

  //Sebesség számítása + rekurzív szűrés sebességre
  aggrSpeedLeft = (oldPart * aggrSpeedLeft + newPart * 1000 * (leftPos - leftPosOld)) / wholePart;
  aggrSpeedRight = (oldPart * aggrSpeedRight + newPart * 1000 * (rightPos - rightPosOld)) / wholePart;

  int opLeft = PTagPos * errorLeft - DTagPos * aggrSpeedLeft;
  int opRight = PTagPos * errorRight - DTagPos * aggrSpeedRight;
  
  //Tárolóba pozíció mentés
  leftPosOld = leftPos;
  rightPosOld = rightPos;
  
  SetMotorPower(opLeft, opRight);
}


//Call this to setup drive system
void SetupMotorAutomation() {
  SetupMotors();
}

