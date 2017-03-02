
#define Ptag 3
#define Dtag 0
#define Itag 0

#define myinterval 5000
#define accuracy 2

//elapsedMicros elapsedTime;
//double aggrSpeedA = 0;
//double aggrSpeedB = 0;
volatile double errDiffAggrA = 0;
volatile double errDiffAggrB = 0;
volatile double errOldA = 0;
volatile double errOldB = 0;
volatile double errSumA = 0;
volatile double errSumB = 0;
IntervalTimer myTimer;

#include "encoderReader.h"

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
  errDiffAggrA = 0.99 * errDiffAggrA + 0.01 * (errorA - errOldA);
  errDiffAggrB = 0.99 * errDiffAggrB + 0.01 * (errorB - errOldB);
  errSumA = 0.9 * errSumA + errorA;
  errSumB = 0.9 * errSumB + errorB;

  double opVA = Ptag * errorA + Dtag * errDiffAggrA + Itag * errSumA;
  double opVB = Ptag * errorB + Dtag * errDiffAggrB + Itag * errSumB;

  errOldA = errorA;
  errOldB = errorB;

  //SetMotorPower(opVA, opVB);
}

void automInterrupt()
{
  SetPos(setPoint, setPoint);
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

