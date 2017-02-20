#include "motorControl.h"

void setup() {
  Serial.begin(9600);
  setupMotors();
}

void loop() {
  if (Serial.available()) {
    delay(50);
    int outputValue = Serial.parseInt();
    setMotorPower(outputValue, outputValue);
    Serial.println(outputValue);
  }
  delay(50);
}
