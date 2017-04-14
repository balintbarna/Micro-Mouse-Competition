//Parameters for infra based speed control
#define PInfra 0.4
#define DInfra 0.2
const int PInfraInverse = 1 / PInfra;
const int DInfraInverse = 1 / DInfra;
const int breakLength = 500;
volatile char nextState = 'S';

volatile int idler = 0;

//Cascade Position
void stateC()
{
  CascadePos(param1, param2);
}
//Stop
void stateS()
{
  //SetMotorSpeed(0, 0);
  SetMotorPower(0, 0);
}
//Velocity control
void stateV()
{
  SetMotorSpeed(param1, param2);
}
//Testing program
void stateT()
{
  ResetAllStoredValues();
  state = 'W';
  param1 = 375;
  param2 = 1500;
}
//Rotating/Turning (pozitive means left)
void stateR()
{
  //végrehajtás
  CascadePos(param1, param2);
  //végállapot ellenőrzés
  if (abs(leftPos) >= abs(param1) && abs(rightPos) >= abs(param2))
  {
    ResetAllStoredValues();
    SetMotorPower(0, 0);
    idler = 0;
    state = 'I';
    nextState = 'T';
  }
}
//Go until wall
void stateW()
{
  if (infra[2] > param2 * 5)
  {
    int de = 0;
    int de_deriv = 0;

    jobboldali = false;
    baloldali = false;

    if (infra[4] < 4500 && infra[3] < 6000)
    {
      jobboldali = true;
      de = 1650 - infra[4];
      de_deriv = infra_deriv[4];
    }
    else if (infra[0] < 4500 && infra[1] < 6000)
    {
      baloldali = true;
      de = infra[0] - 1650;
      de_deriv = -infra_deriv[0];
    }
    int speedL = param1 - de / PInfraInverse + de_deriv / DInfraInverse;
    int speedR = param1 + de / PInfraInverse - de_deriv / DInfraInverse;
    SetMotorSpeed(speedL, speedR);
  }
  else if (infra[2] > param2 + breakLength)
  {
    int targetL = (infra[2] - param2 ) / 100 + leftPos;
    int targetR = (infra[2] - param2 ) / 100 + rightPos;
    CascadePos(targetL, targetR);
  }
  else
  {
    //balra
    if (infra[0] > 9999)
    {
      param1 = -37;
      param2 = 47;
    }
    //jobbra
    else
    {
      param1 = 47;
      param2 = -37;
    }
    idler = 0;
    ResetAllStoredValues();
    SetMotorPower(0, 0);
    state = 'I';
    nextState = 'R';
  }
}
//
void stateD()
{
  ResetAllStoredValues();
  state = 'S';
}
//Waiting/Idle
void stateI()
{
  idler++;
  if (idler > speedMultiplier)
    state = nextState;
}

