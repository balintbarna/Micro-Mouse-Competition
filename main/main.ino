#define DEBUG

#define pwmRes 10

#define infraPin 22

//timer interval (microsec)
#define myinterval 1000

//timer
IntervalTimer myTimer;

const int pwmMax = pow(2, pwmRes);
const int pwmMid = pwmMax / 2;

//setpoint for pid control
volatile int setPoint = 0;

int s0, s1, s2, s3, s4;

#include <Wire.h>
#include "mpu6050.h"
#include "motorAutomation.h"
#include "sensor.h"

//overflower variable
short overF = 0;

//time measure
elapsedMillis elapsedTime = 0;

void setup() {
  //Initialize Serial3 Comm
  Serial.begin(9600);
  Serial3.begin(9600);
  //Analog frekvencia
  analogWriteFrequency(motorLeft, 35156.25);
  analogWriteFrequency(motorRight, 35156.25);
  //Analog 10 biten
  analogWriteResolution(pwmRes);
  //Initialize Motor Automation
  SetupMotorAutomation();
  myTimer.begin(onTimerTick, myinterval);
  //infra
  pinMode(22, OUTPUT);
  digitalWrite(22, 0);
  //led
  pinMode(13, OUTPUT);
}

void loop()
{
  readInfra();
  SerialToValue();
  displayData();
  checkBattery();
}

void checkBattery()
{
  if (analogRead(A14) < 800)
  {
    digitalWrite(13, 1);
  }
  else
    digitalWrite(13, 0);
}

void SerialToValue() {
  if (Serial.available())
  {
    delay(1);
    setPoint = Serial.parseInt();
    elapsedTime = 0;
    Serial.println(setPoint);
  }
  if (Serial3.available())
  {
    delay(1);
    setPoint = Serial3.parseInt();
    elapsedTime = 0;
    Serial3.println(setPoint);
  }
}

void displayData()
{
  String serialop = aggrSpeedLeft;
  serialop += "\t";
  serialop += aggrSpeedRight;
  serialop += "\t";
  serialop += encoderLeft.read();
  serialop += "\t";
  serialop += encoderRight.read();
  serialop += "\t";
  serialop += analogRead(A14);
  serialop += "\t";
  serialop += elapsedTime;

  Serial.println(serialop);
  Serial3.println(serialop);
}

//Functions gets called by timer ticks
void onTimerTick()
{
  CascadePos(setPoint, setPoint);
}

void readInfra()
{
  digitalWrite(infraPin, 1);
  delay(1);
  sensor();
  digitalWrite(infraPin, 0);
}
