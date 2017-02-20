//bal motor pwm
#define aOutP0 9
//jobb motor pwm
#define aOutP1 10
//bal motor enable
#define motorE0 5
//jobb motor enable
#define motorE1 6
//holt tartomány sugara
#define blockRadius 15

double s0 = 0, s1 = 0;

//Call this in setup so you can use the motors
void SetupMotors() {
  pinMode(aOutP0, OUTPUT);
  pinMode(aOutP1, OUTPUT);
  pinMode(motorE0, OUTPUT);
  pinMode(motorE1, OUTPUT);
}

//Speed from -100 to 100, turns off at 0
void SetMotorPower(int speed0, int speed1)
{
  s0 = speed0;
  s1 = speed1;
  double _s0, _s1;
  if (speed0 == 0)
  {
    _s0 = 0;
    digitalWrite(motorE0, 0);
    analogWrite(aOutP0, 127);
  }
  else
  {
    digitalWrite(motorE0, 1);
    if (speed0 < 0)
    {
      _s0 = (double)speed0 * (128 - blockRadius) / 100;
      _s0 += (128 - blockRadius);
    }
    else
    {
      _s0 = (double)speed0 * (129 - blockRadius) / 100;
      _s0 += 126 + blockRadius;
    }
    _s0 = round(_s0);
    analogWrite(aOutP0, _s0);
  }

  if (speed1 == 0)
  {
    _s1 = 0;
    digitalWrite(motorE1, 0);
    analogWrite(aOutP1, 127);
  }
  else
  {
    digitalWrite(motorE1, 1);
    if (speed1 < 0)
    {
      _s1 = (double)speed1 * (128 - blockRadius) / 100;
      _s1 += (128 - blockRadius);
    }
    else
    {
      _s1 = (double)speed1 * (129 - blockRadius) / 100;
      _s1 += 126 + blockRadius;
    }
    _s1 = round(_s1);
    analogWrite(aOutP1, _s1);
  }
}

