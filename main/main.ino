#include "globals.h"

//timer
IntervalTimer myTimer;

//overflower variable
uint16_t overFloop = 0;
uint16_t overFirpt = 0;

#include "includes.h"

void setup()
{
  //Initialize Serial comm
#if DEBUG
  Serial.begin(115200);
  Serial3.begin(115200);
#endif
  //Initialize I2C (for TOF and MPU)
  Wire.begin();
  //Analog frekvencia
  analogWriteFrequency(motorLeft, 35156.25);
  analogWriteFrequency(motorRight, 35156.25);
  //Analog 10 biten
  analogWriteResolution(pwmRes);
  //Initialize Motors
  SetupMotors();
  //Start timer
  myTimer.begin(stateMachine, myinterval);
  //infra
  pinMode(infraPin, OUTPUT);
  digitalWrite(infraPin, 0);
  //button
  pinMode(gombPin, INPUT_PULLUP);
  //leds
  pinMode(led0, OUTPUT);
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  //Initialize TOF sensor
  //SetupTOF();
}

void loop()
{

}

void stateMachine()
{
  jobboldali = false;
  baloldali = false;
  switch (state)
  {
    case 'T':
      stateT();
      break;
    case 'W':
      stateW();
      break;
    case 'C':
      stateC();
      break;
    case 'R':
      stateR();
      break;
    case 'V':
      stateV();
      break;
    case 'S':
      stateS();
      break;
    case 'I':
      stateI();
      break;
    case 'D':
      stateD();
      break;
    case 'O':
      stateS();
      break;
    default:
      state = 'E';
  }
  overFirpt++;
}
