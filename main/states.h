//Parameters for infra based speed control
#define PInfra 0.4
#define DInfra 0.00001
const int PInfraInverse = 1 / PInfra;
const int DInfraInverse = 1 / DInfra;
const int breakLength = 500;

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
  param1 = 300;
  param2 = 2000;
  //  state = 'V';
  //  param1 = 500;
  //  param2 = 500;
}
//Rotating/Turning (pozitive means left)
void stateR()
{
  int leftS = -300;
  int rightS = 300;
  if (leftPos <= -param1)
    leftS = 0;
  if (rightPos >= param1)
    rightS = 0;
  SetMotorSpeed(leftS, rightS);
  if (leftPos <= -param1 && rightPos >= param1)
  {
    idler = 0;
    state = 'T';
  }
}
//delete values
void stateD()
{
  ResetAllStoredValues();
  state = 'S';
}
//Go until wall
void stateW()
{
  if (infra[2] > param2 * 5)
  {
    int de = (1650 - infra[4]);
    int de_deriv = infra_deriv[4];
    int speedL = param1 - de / PInfraInverse;
    int speedR = param1 + de / PInfraInverse;
    //int speedL = param1 - de / PInfraInverse + de_deriv / DInfraInverse;
    //int speedR = param1 + de / PInfraInverse - de_deriv / DInfraInverse;
    SetMotorSpeed(speedL, speedR);
  }
  else if (infra[2] > param2 + breakLength)
  {
    int target = (infra[2] - param2 - 500) / 100 + leftPos;
    CascadePos(target, target);
  }
  else
  {
    idler = 0;
    param1 = 35;
    ResetAllStoredValues();
    SetMotorPower(0, 0);
    state = 'S';
  }
}
//Waiting/Idle
void stateI()
{
  idler++;
  if (idler > speedMultiplier)
    state = 'R';
}

