//Parameters for infra based speed control
volatile char nextState = 'S';

volatile bool pulled = false;

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

  setOrientation(degree / 45);
  planningDone = false;
  state = 'R';
  //nextState = 'R';
}

//Function to erase past stored values
void ResetAllStoredValues()
{
  ResetEncoders();
  ResetMovement();
  ResetLocation();
  idler = 0;
}

void SetAllToDefault()
{
  pos.x = 0;
  pos.y = 0;
  savedPos.x = 0;
  savedPos.y = 0;
  orientation = 0;
  cellMidZone = true;
  for (int i = 0; i < mapsize - 1; i++)
  {
    xWalls[i] = 0;
    yWalls[i] = 0;
  }
  for (int i = 0; i < mapsize; i++)
  {
    visited[i] = 0;
  }
  state = 'S';
  nextState = 'S';
}

bool shouldTurn()
{
  if (cellMidZone)
  {
    int posEncAvg = (encoderLeft.read() + encoderRight.read()) / 2;
    int distance = posEncAvg - lastPosEncAvg;
    distance = (abs(pos.x - savedPos.x) + abs(pos.y - savedPos.y)) * cell_length - distance;
    if (distance > cell_length / 50)
      return false;

    if (!planningDone)
      return false;

    if (nextStep == 'F')
      return false;

    return true;
  }
  return false;
}

bool FollowPath()
{
  if (cellMidZone)
  {
    if (pulled)
      return false;

    int posEncAvg = (encoderLeft.read() + encoderRight.read()) / 2;
    int distance = posEncAvg - lastPosEncAvg;
    distance = (abs(pos.x - savedPos.x) + abs(pos.y - savedPos.y)) * cell_length - distance;
    if (distance > cell_length / 50)
      return false;

    if (!path.length())
      return false;

    if (!planningDone)
      return false;

    nextStep = path[0];
    //path = path.substring(1);
    pulled = true;
    return true;
    if (nextStep == 'F')
    {
      //nothing
      return false;
    }
    else if (nextStep == 'R')
    {
      //jobbra

    }
    else if (nextStep == 'L')
    {
      //balra

    }
    else if (nextStep == 'B')
    {
      //hátra

    }
    return true;

  }
  return false;
}

//---------------- STATES ---------------
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
  if (shouldTurn())
  {
    ResetAllStoredValues();
    SetMotorPower(0, 0);
    if (nextStep == 'L')
      setTurn(-90);
    else if (nextStep == 'R')
      setTurn(90);
    else if (nextStep == 'B')
      setTurn(180);
  }
  else
  {
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
    state = 'T';
    pulled = false;
  }
}
//
void stateD()
{
  ResetAllStoredValues();
  SetAllToDefault();
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
void stateGoalSet()
{
  goal.x = param1;
  goal.y = param2;
  state = 'T';
}

