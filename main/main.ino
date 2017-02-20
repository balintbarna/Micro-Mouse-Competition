#include "motorAutomation.h"
byte overflower = 0;

void setup() {
  //Initialize Serial Comm
  Serial.begin(115200);
  //Initialize Motor Automation
  SetupMororAutomation();
}

void loop() {
  if (overflower == 0)
    displaySpeeds();
  serialToMotors();
  overflower++;
  delay(10);
}

void serialToMotors() {
  if (Serial.available()) {
    delay(10);
    int outputValue = Serial.parseInt();
    SetMotorPower(outputValue, outputValue);
    Serial.println(outputValue);
  }
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

