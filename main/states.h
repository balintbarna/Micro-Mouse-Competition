//Parameters for infra based speed control
volatile char nextState = 'S';

volatile int idler = 0;
#define waitTime 0 //in ms
const int waitCycle = waitTime * timerFrequency / 1000;

//Function to prepare for turning
void setTurn(int16_t degree)
{
  if (!degree) return;

  int ratio = 360 / abs(degree);
  //jobbra
  if (degree > 0)
  {
    param1 = positiveFullRotation / ratio;
    param2 = negativeFullRotation / ratio;
  }
  //balra
  else
  {
    param1 = negativeFullRotation / ratio;
    param2 = positiveFullRotation / ratio;
  }
  if (true)
  {
    /*
    if (degree == 90)
    {
      param1 = 150;
      param2 = -126;

    }
    if (degree == -90)
    {
      param1 = -126;
      param2 = 150;
    }
    */
    if (degree == 180)
    {
      param1 = 282;
      param2 = -252;
    }
  }

  turn(degree / 45);
  state = 'I';
  nextState = 'R';
}

//Function to erase past stored values
void ResetAllStoredValues()
{
  ResetEncoders();
  ResetMovement();
  ResetLocation();
  idler = 0;
}

//---------------- STATES ---------------
//State that decides what to do next
void stateAI()
{
  if (action[0] == 'F')
  {
    state = 'G';
  }
  else if (action[0] == 'R')
  {
    setTurn(90);
  }
  else if (action[0] == 'L')
  {
    setTurn(-90);
  }
  nextState = 'A';
}
//Reach a coordinate, no turning
void stateG()
{
  int posEncAvg = (encoderLeft.read() + encoderRight.read()) / 2;
  int distance = posEncAvg - lastPosEncAvg;
  distance = abs(posX - savedPosX) + abs(posY - savedPosY) * cell_length - distance;
  if (nextX != posX && nextY != posY)
  {
    SetMotorSpeed(maxSpeed, maxSpeed, true);
  }
  else
  {
    if (!cellMidZone)
    {
      int targetL = distance + leftPos;
      int targetR = distance + rightPos;
      CascadePos(targetL, targetR, true);
    }
    else
    {
      ResetAllStoredValues();
      SetMotorPower(0, 0);
      state = nextState;
    }
  }
}
//Testing program
void stateT()
{
  ResetAllStoredValues();
  state = 'W';
  param1 = maxSpeed;
  param2 = frontInfraLimit;
}
//Go until wall
void stateW()
{
  checkWalls();
  updatePosition();
  //Még mehetünk egyenesen bőven
  if (infra[front] > param2 * 4)
  {
    SetMotorSpeed(param1, param1, true);
  }
  //Lassan meg kell állni mert már látok szembe falat
  else if (infra[front] > (param2 + breakLengthInfra))
  {
    int targetL = (infra[front] - param2 ) / encoderToInfra + leftPos;
    int targetR = (infra[front] - param2 ) / encoderToInfra + rightPos;
    CascadePos(targetL, targetR, true);
  }
  //Most azonnal meg kell állni mert féktávolságon belül van a megállási pont
  else
  {
    ResetAllStoredValues();
    SetMotorPower(0, 0);
    //Ha balra nincs fal, arra fordul
    if (infra[left] > 4000)
    {
      setTurn(-90);
    }
    //Ha jobbra nincs fal
    else if (infra[right] > 4000)
    {
      setTurn(90);
    }
    //Egyébként megfordul
    else
    {
      setTurn(180);
    }
  }
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
    state = 'I';
    nextState = 'T';
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
//Cascade Position
void stateC()
{
  CascadePos(param1, param2, true);
}
//Velocity control
void stateV()
{
  SetMotorSpeed(param1, param2, true);
}
//Stop
void stateS()
{
  if (abs(aggrSpeedLeft) > 30 || abs(aggrSpeedRight) > 30)
    SetMotorSpeed(0, 0);
  else
  {
    ResetAllStoredValues();
    SetMotorPower(0, 0);
  }
}
