//bal motor pwm
#define motorLeft 3
//jobb motor pwm
#define motorRight 5
//bal motor enable
#define motorLeftE 4
//jobb motor enable
#define motorRightE 6
//max power stuff
#define powerDuty 0.7
#define absoluteMaxPower 1000000
//maximum érték (max 1000000)
const int maxPower = absoluteMaxPower * powerDuty;

//Call this in setup so you can use the motors
void SetupMotors() {
  pinMode(motorLeft, OUTPUT);
  pinMode(motorRight, OUTPUT);
  pinMode(motorLeftE, OUTPUT);
  pinMode(motorRightE, OUTPUT);
}


//Torque from -1000000 to 1000000, turns off at 0
void SetMotorPower(int powerLeft, int powerRight)
{
  //Comparing and limiting the values to maxPower
  if (abs(powerLeft) > maxPower)
  {
    int ratio = 2000 * maxPower / abs(powerLeft);
    powerLeft *= ratio;
    powerLeft /= 2000;
    powerRight *= ratio;
    powerRight /= 2000;
  }
  if (abs(powerRight) > maxPower)
  {
    int ratio = 2000 * maxPower / abs(powerRight);
    powerLeft *= ratio;
    powerLeft /= 2000;
    powerRight *= ratio;
    powerRight /= 2000;
  }

  //Left Motor
  //If the input is 0, the motor gets grounded
  if (powerLeft == 0)
  {
    digitalWrite(motorLeftE, 0);
    analogWrite(motorLeft, 127);
  }
  else
  {
    digitalWrite(motorLeftE, 1);
    //Backward
    //If the input is negative, the pwm duty maps to larger than half (directions are inverted)
    if (powerLeft < 0)
    {
      powerLeft = map(powerLeft, -absoluteMaxPower, -1, 0, pwmMid);
    }
    //Forward
    //If the input is positive, the pwm duty maps to smaller than half (directions are inverted)
    else
    {
      powerLeft = map(powerLeft, 1, absoluteMaxPower, pwmMid, pwmMax);
    }
    analogWrite(motorLeft, powerLeft);
  }

  //Right motor
  if (powerRight == 0)
  {
    digitalWrite(motorRightE, 0);
    analogWrite(motorRight, 127);
  }
  else
  {
    digitalWrite(motorRightE, 1);
    //Backward
    if (powerRight < 0)
    {
      powerRight = map(powerRight, -absoluteMaxPower, -1, 0, pwmMid);
    }
    //Forward
    else
    {
      powerRight = map(powerRight, 1, absoluteMaxPower, pwmMid, pwmMax);
    }
    analogWrite(motorRight, powerRight);
  }
}
