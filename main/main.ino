#define DEBUG

#define pwmRes 10

//timer interval (microsec)
#define myinterval 1000

//timer
IntervalTimer myTimer;

const int pwmMax = pow(2, pwmRes);
const int pwmMid = pwmMax / 2;

int setPoint = 0;

#include "motorAutomation.h"

//overflower variable
short overF = 0;

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
    setPoint = Serial.parseInt();
    Serial.println(setPoint);
  }
  if (Serial3.available())
  {
    delay(1);
    setPoint = Serial3.parseInt();
    Serial3.println(setPoint);
  }
}

void displayData()
{
  if (!overF)
    //if(1)
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

    Serial.println(serialop);
    Serial3.println(serialop);
  }
  overF++;
}


//Functions gets called by timer ticks
void onTimerTick()
{
  //SetPos(setPoint, setPoint);
  SetMotorSpeed(setPoint, setPoint);
  //SetMotorPower(setPoint,setPoint);
}
