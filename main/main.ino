#include "globals.h"

//timer
IntervalTimer stateTimer;
IntervalTimer infraTimer;

//overflower variable
uint16_t overFloop = 0;
volatile uint16_t overFirpt = 0;

volatile bool sendInfo = false;

#include "includes.h"

//---------------- SETUP ---------------
void setup()
{
  //Initialize Serial comm
#if DEBUG
  Serial.begin(115200);
  Serial3.begin(115200);
#endif
  //Initialize I2C (for TOF and MPU)
  //Wire.begin();
  //Init MPU
  //SetupMPU();
  //Analog frekvencia
  analogWriteFrequency(motorLeft, 35156.25);
  analogWriteFrequency(motorRight, 35156.25);
  //Analog 10 biten
  analogWriteResolution(pwmRes);
  //Initialize Motors
  SetupMotors();
  //Set timer priorities
  //stateTimer.priority(100);
  //infraTimer.priority(105);
  stateTimer.begin(stateMachine, myinterval);
  infraTimer.begin(InfraISR, 1000);
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

//---------------- LOOP ----------------
void loop()
{
  delayTimer = 0;
#if DEBUG
  //digitalWrite(led1, leftwall_debug);
  //digitalWrite(led2, rightwall_debug);
  //digitalWrite(led2, shouldPlan);

  serialToValue();

  //sendInfo = true;
  if (sendInfo)
  {
    displayData();
    sendInfo = false;
  }

#endif
  checkBattery();
  if (!digitalRead(gombPin))
  {
    while (delayTimer < 300);
    //setYawCorrection();
    //    if (!infraon)
    //    {
    //      //_calibrateInfra();
    //      infraTimer.begin(InfraISR, 1000);
    //      infraon = true;
    //    }
    state = 'T';
  }
  //readTurnError();
  if (!infraMidZone && shouldPlan)
  {
    PlanNextStep();
    //PlanPathToTarget();
    shouldPlan = false;
  }
  overFloop++;
  while (delayTimer < 2);
}

void checkBattery()
{
  if (analogRead(batteryPin) < 800)
  {
    digitalWrite(led0, 1);
    if (analogRead(batteryPin) < 780)
    {
      //stateTimer.end();
      //infraTimer.end();
      //state = 'O';
      //SetMotorPower(0, 0);
    }
  }
  else
    digitalWrite(led0, 0);
}

//-------------- TIMER INTERRUPT --------------
void stateMachine()
{
  switch (state)
  {
    case 'G':
      stateGoalSet();
      break;
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
    case 'I':
      stateI();
      break;
    case 'D':
      stateD();
      break;
    case 'S':
    case 'E':
    case 'O':
      stateS();
      break;
    default:
      state = 'E';
  }
  //Ha letelt kb 5 perc
  if (milli > 280000)
  {
    state = 'S';
  }
  //Ha letelt kb 4 perc
  else if (milli > 240000)
  {
    //Set goal
    goal.x = 0;
    goal.y = 0;
    //stop
    if (cellMidZone && !pos.x && !pos.y)
    {
      state = 'S';
      digitalWrite(led1, 0);
      digitalWrite(led2, 0);
    }
    else
    {
      digitalWrite(led1, 0);
      digitalWrite(led2, 1);
    }
  }
  //ha midzone
  else if (cellMidZone && pos.x == goal.x && pos.y == goal.y)
  {
    //Startnál van
    if (!pos.x && !pos.y)
    {
      //Set goal
      goal.x = originalGoalX;
      goal.y = originalGoalY;
      //debug
      digitalWrite(led1, 1);
      digitalWrite(led2, 0);
      sendInfo = true;
      shouldPlan = true;
    }
    //Célbaért
    else
    {
      //Set goal
      goal.x = 0;
      goal.y = 0;
      //debug
      digitalWrite(led1, 0);
      digitalWrite(led2, 1);
      sendInfo = true;
      shouldPlan = true;
    }
  }
  overFirpt++;
}
