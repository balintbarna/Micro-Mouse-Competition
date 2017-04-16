//Parameters for infra based speed control
const int breakLength = maxSpeed;
volatile char nextState = 'S';

volatile int idler = 0;

#define waitTime 100 //in ms
const int waitCycle = waitTime * timerFrequency / 1000;

//Cascade Position
void stateC()
{
  CascadePos(param1, param2);
}
//Stop
void stateS()
{
  if (abs(aggrSpeedLeft) > 30 || abs(aggrSpeedRight) > 30)
    SetMotorSpeed(0, 0);
  else
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
  param1 = maxSpeed;
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
  //Még mehetünk egyenesen bőven
  if (infra[front] > param2 * 4)
  {
    SetMotorSpeed(param1, param1, true);
  }
  //Lassan meg kell állni mert már látok szembe falat
  else if (infra[front] > param2 + breakLength)
  {
    int targetL = (infra[front] - param2 ) / 100 + leftPos;
    int targetR = (infra[front] - param2 ) / 100 + rightPos;
    CascadePos(targetL, targetR, true);
  }
  //Most azonnal meg kell állni mert féktávolságon belül van a megállási pont
  else
  {
    //Ha balra nincs fal, arra fordul
    if (infra[left] > 9999)
    {
      param1 = -37;
      param2 = 47;
    }
    //Egyébként jobbra
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
  if (idler > waitCycle)
    state = nextState;
}

