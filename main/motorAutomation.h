#define multiplier 1.68
#define maxPower 100
#define P 0.01

#include "encoderReader.h"

unsigned long lastTime = 0;

//Call this to setup drive system
void SetupMororAutomation() {
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
  if (!overflower);
  Serial.print("Left: ");
  Serial.print(speed0);
  Serial.print(" Right: ");
  Serial.println(speed1);
#endif
  double error0 = goSpeed0 - speed0;
  speed0 = s0 + error0 * P;

  SetMotorPower(speed0, 0);

#ifdef DEBUG
  if (!overflower);
  Serial.print("Level: ");
  Serial.println(speed0);
#endif
  overflower++;

}




