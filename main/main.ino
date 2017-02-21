#define DEBUG
byte overflower = 0;
#include "motorAutomation.h"
int outputValue = 0;

void setup() {
  //Initialize Serial Comm
  Serial.begin(115200);
  //Analog frekvencia
  analogWriteFrequency(aOutP0, 8789.062);
  analogWriteFrequency(aOutP1, 8789.062);
  //Analog 12 biten
  analogWriteResolution(12);
  //Initialize Motor Automation
  SetupMotorAutomation();
}

void loop() {
//  if (overflower == 0)
//    displaySpeeds();
  serialToMotors();
//  overflower++;
  delay(30);
}

void serialToMotors() {
  if (Serial.available()) {
    delay(10);
    outputValue = Serial.parseFloat();
    //SetMotorPower(outputValue, outputValue);
    Serial.println(outputValue);
  }
  SetSpeed(outputValue, 0);
}

void displaySpeeds() {
  double _time = (micros() - lastTime) / 1000000.0;
  double speed0 = getSpeed0(_time);
  double speed1 = getSpeed1(_time);
  lastTime = micros();
  Serial.print("Left: ");
  Serial.print(speed0);
  Serial.print(" Right: ");
  Serial.println(speed1);
}

