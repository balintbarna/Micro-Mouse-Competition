//bal motor encoder bemenet
#define irptP0 16
//jobb motor encoder bemenet
#define irptP1 18
//bal motor encoder számláló
volatile signed long encoder0 = 0;
volatile signed long lastPos0 = 0;
//jobb motor encoder számláló
volatile signed long encoder1 = 0;
volatile signed long lastPos1 = 0;

#include "motorControl.h"

//bal motor számláló függvény
void add0() {
  if (s0 > 0)
    encoder0++;
  else
    encoder0--;
}
//jobb motor számláló függvény
void add1() {
  if (s1 > 0)
    encoder1++;
  else
    encoder1--;
}

//Call this to setup pins for encoders
void SetupEncoders() {
  pinMode(irptP0, INPUT_PULLUP);
  pinMode(irptP1, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(irptP0), add0, RISING);
  attachInterrupt(digitalPinToInterrupt(irptP1), add1, RISING);
}

//Resets encoder values
void ResetEncoders() {
  encoder0 = 0;
  lastPos0 = 0;
  encoder1 = 0;
  lastPos1 = 0;
}




