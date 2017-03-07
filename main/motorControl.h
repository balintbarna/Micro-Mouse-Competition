//bal motor pwm
#define motorA 23
//jobb motor pwm
#define motorB 22
//bal motor enable
#define motorEA 5
//jobb motor enable
#define motorEB 6
//holt tartomány sugara
#define blockRadius 58
//maximum érték (max 100)
#define maxPower 30
//maximum gyorsulás
//#define maxTorqueC 0.0002
#define maxTorqueC 0.0005
const double maxTorque = myinterval * maxTorqueC;

#include "myFunctions.h"

const int negLowBound = pwmMid - blockRadius;
const int posLowBound = pwmMid + blockRadius;

volatile double sA = 0, sB = 0;

//Call this in setup so you can use the motors
void SetupMotors() {
  pinMode(motorA, OUTPUT);
  pinMode(motorB, OUTPUT);
  pinMode(motorEA, OUTPUT);
  pinMode(motorEB, OUTPUT);
}


//Torque from -100 to 100, turns off at 0
void SetMotorPower(double speedA, double speedB)
{
  //Comparing and setting the values to maxPower
  if (abs(speedA) > maxPower)
  {
    double ratio = maxPower / abs(speedA);
    speedA *= ratio;
    speedB *= ratio;
  }
  if (abs(speedB) > maxPower)
  {
    double ratio = maxPower / abs(speedB);
    speedA *= ratio;
    speedB *= ratio;
  }

  //Making sure that velocity doesn't increase too fast
  //if (abs(speedA - sA) > maxTorque) speedA = sA + sign(speedA - sA) * maxTorque;
  //if (abs(speedB - sB) > maxTorque) speedB = sB + sign(speedB - sB) * maxTorque;


  sA = speedA;
  sB = speedB;
  if (abs(speedA) < 1)
  {
    digitalWrite(motorEA, 0);
    analogWrite(motorA, 127);
  }
  else
  {
    digitalWrite(motorEA, 1);
    if (speedA < 0)
    {
      speedA = mapfloat(speedA, -100, -1, 0, negLowBound);
    }
    else
    {
      speedA = mapfloat(speedA, 1, 100, posLowBound, pwmMax);
    }
    int _speedA = round(speedA);
    analogWrite(motorA, _speedA);
  }

  if (speedB == 0)
  {
    digitalWrite(motorEB, 0);
    analogWrite(motorB, 127);
  }
  else
  {
    digitalWrite(motorEB, 1);
    if (speedB < 0)
    {
      speedB = mapfloat(speedB, -100, -1, 0, negLowBound);
    }
    else
    {
      speedB = mapfloat(speedB, 1, 100, posLowBound, pwmMax);
    }
    int _speedB = round(speedB);
    analogWrite(motorB, _speedB);
  }
}

