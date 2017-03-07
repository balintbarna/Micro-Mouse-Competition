//PID controllers
#define Ptag 0.1
#define Dtag 0.1
#define Itag 0.1

//timer interval (microsec)
#define myinterval 5000

//position control accuracy
#define accuracy 1

//constants for recursive filter
#define newPart 0.1
const double oldPart = 1 - newPart;

//maximum acceleration (speed change in a cycle)
#define maxAccelC 0.00001
const double maxAccel = myinterval * maxAccelC;

//Tárolók
//időmérő
//elapsedMicros elapsedTime;
//szűrt sebesség hiba
volatile double aggrSpeedA = 0;
volatile double aggrSpeedB = 0;
//szűrt pozívió hiba derivált
volatile double errDiffAggrA = 0;
volatile double errDiffAggrB = 0;
//régi hiba
volatile double errOldA = 0;
volatile double errOldB = 0;
//integráló tag
volatile double errSumA = 0;
volatile double errSumB = 0;
//régi setspeed
volatile double pastSetSpeedA = 0;
volatile double pastSetSpeedB = 0;
//timer
IntervalTimer myTimer;

#include "encoderReader.h"

//Speed control PI
void SetMotorSpeed(double setSpeedA, double setSpeedB)
{
  //Maximum gyorsulás beállítása
  //if (abs(setSpeedA - pastSetSpeedA) > maxAccel) setSpeedA = pastSetSpeedA + sign(setSpeedA - pastSetSpeedA) * maxAccel;
  //if (abs(setSpeedB - pastSetSpeedB) > maxAccel) setSpeedB = pastSetSpeedB + sign(setSpeedB - pastSetSpeedB) * maxAccel;

  //Sebesség számítása + rekurzív szűrés sebességre
  aggrSpeedA = oldPart * aggrSpeedA + newPart * (distanceA - pastA);
  aggrSpeedB = oldPart * aggrSpeedB + newPart * (distanceB - pastB);

  //0 közeli hiba kiiktatása
  if (abs(aggrSpeedA) < 0.00001)
  {
    aggrSpeedA = 0;
  }
  if (abs(aggrSpeedB) < 0.00001)
  {
    aggrSpeedB = 0;
  }

  //Sebesség hiba számítása
  double errorA = setSpeedA - aggrSpeedA;
  double errorB = setSpeedB - aggrSpeedB;

  //Tárolóba pozíció mentés
  pastA = distanceA;
  pastB = distanceB;

  //Integráló tag növelése
  errSumA += errorA;
  errSumB += errorB;

  //Tárolóba setSpeet metése
  pastSetSpeedA = setSpeedA;
  pastSetSpeedB = setSpeedB;

  //double opVA = sA + PtagSpeed * errorA + Itag * errSumA;
  //double opVB = sB + PtagSpeed * errorB + Itag * errSumB;
  double opVA = Ptag * errorA + Itag * errSumA;
  double opVB = Ptag * errorB + Itag * errSumB;

  SetMotorPower(opVA, opVB);
}

//Position control (incremental) PD
void SetPos(double spA, double spB)
{
  double errorA = spA - distanceA;
  double errorB = spB - distanceB;
  if (abs(errorA) < accuracy)
  {
    errorA = 0;
    errSumA = 0;
    errDiffAggrA = 0;
  }
  if (abs(errorB) < accuracy)
  {
    errorB = 0;
    errSumB = 0;
    errDiffAggrB = 0;
  }
  errDiffAggrA = 0.9 * errDiffAggrA + 0.1 * (errorA - errOldA);
  errDiffAggrB = 0.9 * errDiffAggrB + 0.1 * (errorB - errOldB);
  errSumA = 0.9 * errSumA + errorA;
  errSumB = 0.9 * errSumB + errorB;

  double opVA = Ptag * errorA + Dtag * errDiffAggrA;
  double opVB = Ptag * errorB + Dtag * errDiffAggrB;

  errOldA = errorA;
  errOldB = errorB;

  SetMotorSpeed(opVA, opVB);
}

//Functions gets called by timer ticks
void automInterrupt()
{
  //SetPos(setPoint, setPoint);
  SetMotorSpeed(setPoint, setPoint);
  //SetMotorPower(setPoint, setPoint);
}

//Call this to setup drive system
void SetupMotorAutomation() {
  SetupMotors();
  SetupEncoders();
  //  elapsedTime = 0;
  myTimer.begin(automInterrupt, myinterval);
}



////Gets left wheel's speed in mm/s
//double getSpeedA(double Time)
//{
//  double speedA = distanceA - pastA;
//  speedA /= Time;
//  aggrSpeedA = 0.9 * aggrSpeedA + 0.1 * speedA;
//  pastA = distanceA;
//  return aggrSpeedA;
//}
////Gets right wheel's speed in mm/s
//double getSpeedB(double Time)
//{
//  double speedB = distanceB - pastB;
//  speedB /= Time;
//  aggrSpeedB = 0.9 * aggrSpeedB + 0.1 * speedB;
//  pastB = distanceB;
//  return aggrSpeedB;
//}

////Sets speed in mm/sec
//void SetSpeed(double goSpeed0, double goSpeed1)
//{
//  double _time = elapsedTime / 1000000.0;
//  double speedA = getSpeedA(_time);
//  double speedB = getSpeedB(_time);
//  elapsedTime=0;
//#ifdef DEBUG
//  if (!overflower) {
//    Serial3.println("Speed");
//    Serial3.print("Left: ");
//    Serial3.print(speed0);
//    Serial3.print(" Right: ");
//    Serial3.println(speed1);
//  }
//#endif
//
//  speed0 = s0 + (goSpeed0 - speed0) * P;
//  speed1 = s1 + (goSpeed1 - speed1) * P;
//
//  if (!goSpeed0) speed0 = 0;
//  if (!goSpeed1) speed1 = 0;
//  SetMotorPower(speed0, speed1);
//
//
//#ifdef DEBUG
//  if (!overflower) {
//    Serial3.println("Level");
//    Serial3.print("Left: ");
//    Serial3.print(speed0);
//    Serial3.print(" Right: ");
//    Serial3.println(speed1);
//  }
//#endif
//  overflower++;
//
//}


//void SetPos(double posA, double posB)
//{
//    double _time = elapsedTime / 1000000.0;
//    double speedA = getSpeedA(_time);
//    double speedB = getSpeedB(_time);
//    elapsedTime = 0;
//
//  double opVA;
//  double opVB;
//  double errorA = posA - distanceA;
//  double errorB = posB - distanceB;
//  if (abs(errorA) < accuracy)
//    errorA = 0;
//  if (abs(errorB) < accuracy)
//    errorB = 0;
//    opVA = Ptag * errorA + Dtag * speedA;
//    opVB = Ptag * errorB + Dtag * speedB;
//
//  SetMotorPower(opVA, opVB);
//
//}

