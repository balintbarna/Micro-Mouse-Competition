#define multiplier 1.68
#define P 0.001

#include "encoderReader.h"

unsigned long lastTime = 0;

//Call this to setup drive system
void SetupMotorAutomation() {
  SetupMotors();
  SetupEncoders();
  lastTime = micros();
}

//Gets left wheel's speed in mm/s
double getSpeed0(double Time)
{
  double speed0 = encoder0 - lastPos0;
  speed0 *= multiplier;
  speed0 /= Time;

  lastPos0 = encoder0;
  return speed0;
}
//Gets right wheel's speed in mm/s
double getSpeed1(double Time)
{
  double speed1 = encoder1 - lastPos1;
  speed1 *= multiplier;
  speed1 /= Time;

  lastPos1 = encoder1;
  return speed1;
}

//Sets speed in mm/sec
void SetSpeed(double goSpeed0, double goSpeed1)
{
  double _time = (micros() - lastTime) / 1000000.0;
  double speed0 = getSpeed0(_time);
  double speed1 = getSpeed1(_time);
  lastTime = micros();
#ifdef DEBUG
  if (!overflower) {
    Serial.print("Left: ");
    Serial.print(speed0);
    Serial.print(" Right: ");
    Serial.println(speed1);
  }
#endif
  if (speed1 == goSpeed1)
  {
    speed1 = s1;
  }
  else if (speed1 < goSpeed1)
  {
    speed1 = s1 + 1;
  }
  else
  {
    speed1 = s1 - 1;
  }
  //speed0 = s0 + (goSpeed0 - speed0) * P;
  //speed1 = s1 + (goSpeed1 - speed1) * P;

  if (!goSpeed0) speed0 = 0;
  if (!goSpeed1) speed1 = 0;
  SetMotorPower(speed0, speed1);


#ifdef DEBUG
  if (!overflower) {
    Serial.print("Level Left: ");
    Serial.print(speed0);
    Serial.print(" Right: ");
    Serial.println(speed1);
  }
#endif
  overflower++;

}




