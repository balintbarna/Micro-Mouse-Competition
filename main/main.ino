#define DEBUG
#define pwmRes 10
const int pwmMax = pow(2, pwmRes);
const int pwmMid = pwmMax / 2;
double setPoint = 0;
#include "motorAutomation.h"
short overF = 0;

void setup() {
  //Initialize Serial3 Comm
  Serial.begin(9600);
  Serial3.begin(9600);
  //Analog frekvencia
  analogWriteFrequency(motorA, 35156.25);
  analogWriteFrequency(motorB, 35156.25);
  //Analog 12 biten
  analogWriteResolution(pwmRes);
  //Initialize Motor Automation
  SetupMotorAutomation();
}

void loop()
{
  SerialToValue();
  displayData();
}

void SerialToValue() {
  if (Serial.available())
  {
    delay(1);
    setPoint = Serial.parseFloat();
    Serial.println(setPoint);
  }
  if (Serial3.available())
  {
    delay(1);
    setPoint = Serial3.parseFloat();
    Serial3.println(setPoint);
  }
}

void displayData()
{
  if (!overF)
  //if(1)
  {
    String serialop = (aggrSpeedA);
    serialop += ";";
    serialop += aggrSpeedB;
    serialop += ";";
    serialop += distanceA;
    serialop += ";";
    serialop += distanceB;

    Serial.println(serialop);
    Serial3.println(serialop);
  }
  overF++;
}

//void displaySpeeds() {
//  double _time = (micros() - lastTime) / 1000000.0;
//  double speed0 = getSpeed0(_time);
//  double speed1 = getSpeed1(_time);
//  lastTime = micros();
//  Serial3.print("Left: ");
//  Serial3.print(speed0);
//  Serial3.print(" Right: ");
//  Serial3.println(speed1);
//}

