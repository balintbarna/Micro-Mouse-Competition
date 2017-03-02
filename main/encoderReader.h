//bal motor encoder bemenet
#define encoderA0 18
#define encoderA1 19
//jobb motor encoder bemenet
#define encoderB0 16
#define encoderB1 17
//bal motor encoder számláló
volatile double distanceA = 0;
volatile double pastA = 0;
//jobb motor encoder számláló
volatile double distanceB = 0;
volatile double pastB = 0;

#include "motorControl.h"

//bal motor számláló függvény
void addA() {
  if (sA > 0)
    distanceA+=0.84;
  else
    distanceA-=0.84;
}
//jobb motor számláló függvény
void addB() {
  if (sB > 0)
    distanceB+=0.84;
  else
    distanceB-=0.84;
}

//Call this to setup pins for encoders
void SetupEncoders() {
  pinMode(encoderA0, INPUT_PULLUP);
  pinMode(encoderA1, INPUT_PULLUP);
  pinMode(encoderB0, INPUT_PULLUP);
  pinMode(encoderB1, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(encoderA0), addA, CHANGE);
  attachInterrupt(digitalPinToInterrupt(encoderA1), addA, CHANGE);
  attachInterrupt(digitalPinToInterrupt(encoderB0), addB, CHANGE);
  attachInterrupt(digitalPinToInterrupt(encoderB1), addB, CHANGE);
}

//Resets encoder values
void ResetEncoders() {
  distanceA = 0;
  pastA = 0;
  distanceB = 0;
  pastB = 0;
}




