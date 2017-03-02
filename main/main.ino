#define DEBUG
double setPoint = 0;
#include "motorAutomation.h"
short overF = 0;

void setup() {
  //Initialize Serial3 Comm
  Serial.begin(9600);
  Serial3.begin(9600);
  //Analog frekvencia
  analogWriteFrequency(motorA, 8789.062);
  analogWriteFrequency(motorB, 8789.062);
  //Analog 12 biten
  analogWriteResolution(12);
  //Initialize Motor Automation
  SetupMotorAutomation();  
}

void loop()
{
  SerialToValue();
  displayData();
  SetMotorPower(setPoint, setPoint);
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
  {
    String serialop = (distanceA);
    serialop += "\t";
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

