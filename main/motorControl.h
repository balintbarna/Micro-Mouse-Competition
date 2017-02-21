//bal motor pwm
#define aOutP0 22
//jobb motor pwm
#define aOutP1 23
//bal motor enable
#define motorE0 6
//jobb motor enable
#define motorE1 5
//holt tartomány sugara
#define blockRadius 240
//maximum érték
#define maxPower 100

#include "myFunctions.h"

const int negLowBound = 2048 - blockRadius;
const int posLowBound = 2048 + blockRadius;

double s0 = 0, s1 = 0;

//Call this in setup so you can use the motors
void SetupMotors() {
  pinMode(aOutP0, OUTPUT);
  pinMode(aOutP1, OUTPUT);
  pinMode(motorE0, OUTPUT);
  pinMode(motorE1, OUTPUT);
}

//Speed from -100 to 100, turns off at 0
void SetMotorPower(double speed0, double speed1)
{
  if (abs(speed0) > maxPower)
  {
    double ratio = maxPower / abs(speed0);
    speed0 *= ratio;
    speed1 *= ratio;
  }
  if (abs(speed1) > maxPower)
  {
    double ratio = maxPower / abs(speed1);
    speed0 *= ratio;
    speed1 *= ratio;
  }
  s0 = speed0;
  s1 = speed1;
  if (speed0 == 0)
  {
    digitalWrite(motorE0, 0);
    analogWrite(aOutP0, 127);
  }
  else
  {
    digitalWrite(motorE0, 1);
    if (speed0 < 0)
    {
      speed0 = mapfloat(speed0, -100, -1, 0, negLowBound);
    }
    else
    {
      speed0 = mapfloat(speed0, 1, 100, posLowBound, 4096);
    }
    int _speed0=round(speed0);
    analogWrite(aOutP0, _speed0);
  }

  if (speed1 == 0)
  {
    digitalWrite(motorE1, 0);
    analogWrite(aOutP1, 127);
  }
  else
  {
    digitalWrite(motorE1, 1);
    if (speed1 < 0)
    {
      speed1 = mapfloat(speed1, -100, -1, 0, negLowBound);
    }
    else
    {
      speed1 = mapfloat(speed1, 1, 100, posLowBound, 4096);
    }
    int _speed1=round(speed1);
    analogWrite(aOutP1, _speed1);
  }
}

