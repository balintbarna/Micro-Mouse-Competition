//bal motor pwm
#define motorLeft 3
//jobb motor pwm
#define motorRight 5
//bal motor enable
#define motorLeftE 4
//jobb motor enable
#define motorRightE 6
//holt tartomány sugara
#define blockRadius 50
//max power stuff
#define powerDuty 0.3
#define absoluteMaxPower 1000000
//maximum érték (max 1000000)
const int maxPower = absoluteMaxPower * powerDuty;
//maximum gyorsulás
//#define maxTorqueC 0.0002
#define maxTorqueC 0.0005
const double maxTorque = myinterval * maxTorqueC;

#include "myFunctions.h"

const int negLowBound = pwmMid - blockRadius;
const int posLowBound = pwmMid + blockRadius;

volatile int powerLeftOld = 0, powerRightOld = 0;

//Call this in setup so you can use the motors
void SetupMotors() {
  pinMode(motorLeft, OUTPUT);
  pinMode(motorRight, OUTPUT);
  pinMode(motorLeftE, OUTPUT);
  pinMode(motorRightE, OUTPUT);
}


//Torque from -100000 to 100000, turns off at 0
void SetMotorPower(int powerLeft, int powerRight)
{
  //Comparing and setting the values to maxPower
  if (abs(powerLeft) > maxPower)
  {
    int ratio = 1000 * maxPower / abs(powerLeft);
    powerLeft *= ratio;
    powerLeft /= 1000;
    powerRight *= ratio;
    powerRight /= 1000;
  }
  if (abs(powerRight) > maxPower)
  {
    int ratio = 1000 * maxPower / abs(powerRight);
    powerLeft *= ratio;
    powerLeft /= 1000;
    powerRight *= ratio;
    powerRight /= 1000;
  }

  //Making sure that velocity doesn't increase too fast
  //if (abs(speedA - sA) > maxTorque) speedA = sA + sign(speedA - sA) * maxTorque;
  //if (abs(speedB - sB) > maxTorque) speedB = sB + sign(speedB - sB) * maxTorque;


  powerLeftOld = powerLeft;
  powerRightOld = powerRight;
  if (abs(powerLeft) == 0)
  {
    digitalWrite(motorLeftE, 0);
    analogWrite(motorLeft, 127);
  }
  else
  {
    digitalWrite(motorLeftE, 1);
    if (powerLeft < 0)
    {
      powerLeft = map(powerLeft, -absoluteMaxPower, -1, pwmMax, posLowBound);
    }
    else
    {
      powerLeft = map(powerLeft, 1, absoluteMaxPower, negLowBound, 0);
    }
    analogWrite(motorLeft, powerLeft);
  }

  if (powerRight == 0)
  {
    digitalWrite(motorRightE, 0);
    analogWrite(motorRight, 127);
  }
  else
  {
    digitalWrite(motorRightE, 1);
    if (powerRight < 0)
    {
      powerRight = map(powerRight, -absoluteMaxPower, -1, pwmMax, posLowBound);
    }
    else
    {
      powerRight = map(powerRight, 1, absoluteMaxPower, negLowBound, 0);
    }
    analogWrite(motorRight, powerRight);
  }
}

