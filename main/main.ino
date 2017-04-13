//---------------- GLOBALS ----------------

#define DEBUG 1

#define infraPin 22
#define batteryPin A14
#define led0 13

//timer
IntervalTimer myTimer;
//timer interval (microsec)
#define myinterval 20000

//Setting pwm output parameters
#define pwmRes 10
const int pwmMax = pow(2, pwmRes);
const int pwmMid = pwmMax / 2;

//setpoint for pid control
volatile int setPoint = 0;

//infra sensor values
volatile int infra[5] = {0, 0, 0, 0, 0};
volatile int pastinfra[5] = {0, 0, 0, 0, 0};
volatile int infra_deriv[5] = {0, 0, 0, 0, 0};

//overflower variable
short overF = 0;

//Variable to read command from serial
String serialCommand = "";
//Paramteres
volatile char state = 'S';
volatile int param1 = 0, param2 = 0, param3 = 0, param4 = 0;

//time measure
elapsedMillis elapsedTime = 0;
elapsedMicros _elapsedMicro = 0;

//Variable for serial output
String serialop = "";


//---------------- INCLUDES ----------------

#include <Wire.h>
#include "mpu6050.h"
#include "encoderReader.h"
#include "motorControl.h"
#include "motorAutomation.h"
#include "myFunctions.h"
#include "infra.h"
#include "states.h"


//---------------- SETUP ----------------
void setup() {
  //Initialize Serial comm
#if DEBUG
  Serial.begin(115200);
  Serial3.begin(115200);
#endif
  //Analog frekvencia
  analogWriteFrequency(motorLeft, 35156.25);
  analogWriteFrequency(motorRight, 35156.25);
  //Analog 10 biten
  analogWriteResolution(pwmRes);
  //Initialize Motors
  SetupMotors();
  //Start timer
  myTimer.begin(onTimerTick, myinterval);
  //infra
  pinMode(infraPin, OUTPUT);
  digitalWrite(infraPin, 0);
  //led
  pinMode(led0, OUTPUT);
  //first infra value
  ReadInfra();
}

//---------------- LOOP ----------------
void loop()
{
#if DEBUG
  serialToValue();
  displayData();
#endif
  checkBattery();
  ReadInfra();
}

void checkBattery()
{
  if (analogRead(batteryPin) < 800)
  {
    digitalWrite(led0, 1);
  }
  else
    digitalWrite(led0, 0);
}

#if DEBUG
void serialToValue() {
  bool newInfo = false;
  if (Serial.available())
  {
    delay(1);
    serialCommand = Serial.readString();
    elapsedTime = 0;
    Serial.println(serialCommand);
    newInfo = true;
  }
  if (Serial3.available())
  {
    delay(1);
    serialCommand = Serial3.readString();
    elapsedTime = 0;
    Serial3.println(serialCommand);
    newInfo = true;
  }
  //If we have new data
  if (newInfo)
  {
    //Read new state
    state = serialCommand[0];
    //Get first space location
    int spaceIndex = serialCommand.indexOf(' ');
    //If no space -> exit
    if (spaceIndex == -1) return;

    //Get string after space
    serialCommand = serialCommand.substring(spaceIndex + 1);
    //Get location of next space
    spaceIndex = serialCommand.indexOf(' ');
    //Get param
    param1 = getParam(spaceIndex, serialCommand).toInt();
    //If no space -> exit
    if (spaceIndex == -1) return;

    //Get string after space
    serialCommand = serialCommand.substring(spaceIndex + 1);
    //Get location of next space
    spaceIndex = serialCommand.indexOf(' ');
    //Get param
    param2 = getParam(spaceIndex, serialCommand).toInt();
    //If no space -> exit
    if (spaceIndex == -1) return;

    //Get string after space
    serialCommand = serialCommand.substring(spaceIndex + 1);
    //Get location of next space
    spaceIndex = serialCommand.indexOf(' ');
    //Get param
    param3 = getParam(spaceIndex, serialCommand).toInt();
    //If no space -> exit
    if (spaceIndex == -1) return;

    //Get string after space
    serialCommand = serialCommand.substring(spaceIndex + 1);
    //Get location of next space
    spaceIndex = serialCommand.indexOf(' ');
    //Get param
    param4 = getParam(spaceIndex, serialCommand).toInt();
    //If no space -> exit
    if (spaceIndex == -1) return;
  }
}

void displayData()
{
  //Speed, Position, Battery, Time
  //  serialop += aggrSpeedLeft;
  //  serialop += "\t";
  //  serialop += aggrSpeedRight;
  //  serialop += "\t";
  //  serialop += leftPos;
  //  serialop += "\t";
  //  serialop += rightPos;
  //  serialop += "\t";
  //  serialop += analogRead(A14);
  //  serialop += "\t";
  //  serialop += elapsedTime;
  //  serialop += "\t";
  //  serialop += _elapsedMicro;
  //  serialop += "\t";

  //Infra sensors
  //  serialop += infra[0];
  //  serialop += "\t";
  //  serialop += infra[1];
  //  serialop += "\t";
  //  serialop += infra[2];
  //  serialop += "\t";
  //  serialop += infra[3];
  //  serialop += "\t";
  serialop += infra[4];
  serialop += "\t";
  serialop += pastinfra[4];
  serialop += "\t";
  serialop += infra_deriv[4];
  serialop += "\t";

  //States and params
  serialop += state;
  serialop += "\t";
  //  serialop += param1;
  //  serialop += "\t";
  //  serialop += param2;
  //  serialop += "\t";
  //  serialop += param3;
  //  serialop += "\t";
  //  serialop += param4;
  //  serialop += "\t";
  //  serialop += idler;
  //  serialop += "\t";

  Serial.println(serialop);
  Serial3.println(serialop);

  serialop = "";
}
#endif

//---------------- TIMER INTERRUPT ----------------
void onTimerTick()
{
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
    case 'D':
      stateD();
      break;
    case 'I':
      stateI();
      break;
    default:
      state = 'E';
  }
}

