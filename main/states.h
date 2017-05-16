//Parameters for infra based speed control
const int breakLength = maxSpeed / 3;
volatile char nextState = 'S';

volatile int idler = 0;
#define waitTime 0 //in ms
const int waitCycle = waitTime * timerFrequency / 1000;

bool computed = false;

void setTurn(int16_t degree)
{
  int ratio = 360 / abs(degree);
  //jobbra
  if (degree >= 0)
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
  for (int i = 0; i < 5; i++)
  {
    infra_deriv[i] = 0;
  }
  idler = 0;
}

void planMovement()
{
  if (midzone)
  {
    //célba jutott
    if (!cellValues[posX][posY])
      state = 'S';
    //még nem jutott célba
    else
    {
      if (computed)
      {
        int temp = getBestDirection(posX, posY);
        int temp2 = temp - orientation;
        if (!temp2)
        {
          ResetAllStoredValues();
          SetMotorPower(0, 0);
          setTurn(temp2 * 45);
        }
      }
    }
  }
}

void checkWalls()
{
  //Szembe van
  if (infra[front] < (frontInfraLimit + breakLength))
    setWall(posX, posY, (orientation / 2) % 4);

  if (midzone)
  {
    //jobbra van
    if (infra[right] < sideInfraLimit && infra[rightdi] < diagonalInfraLimit && pastinfra[right] < sideInfraLimit && infra_deriv[right] < derivInfraLimit)
    {
      setWall(posX, posY, (orientation / 2 + 1) % 4);
    }

    //balra van
    if (infra[left] < sideInfraLimit && infra[leftdi] < diagonalInfraLimit && pastinfra[left] < sideInfraLimit && infra_deriv[left] < derivInfraLimit)
    {
      setWall(posX, posY, (orientation / 2 + 3) % 4);
    }
  }
}

//---------------- STATES ---------------
//Cascade Position
void stateC()
{
  CascadePos(param1, param2, true);
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
  SetMotorSpeed(param1, param2, true);
}
//Testing program
void stateT()
{
  ResetAllStoredValues();
  state = 'W';
  param1 = maxSpeed;
  param2 = frontInfraLimit;
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
//Go until wall
void stateW()
{
  updatePosition();
  checkWalls();
  //planMovement();
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
    ResetAllStoredValues();
    SetMotorPower(0, 0);
    //Ha balra nincs fal, arra fordul
    if (infra[left] > 9999)
    {
      setTurn(-90);
    }
    //Egyébként jobbra
    else
    {
      setTurn(90);
    }
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
