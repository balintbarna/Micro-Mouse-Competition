//Parameters for infra based speed control
const int breakLength = maxSpeed / 3;
volatile char nextState = 'S';

volatile int idler = 0;
#define waitTime 0 //in ms
const int waitCycle = waitTime * timerFrequency / 1000;

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

void checkWalls()
{
  //Szembe van
  if (infra[front] < (1500 + maxSpeed))
    setWall(posX, posY, (orientation / 2) % 4);

  if (midzone)
  {
    //jobbra van
    if (infra[right] < 3000 && infra[rightdi] < 5000 && pastinfra[right] < 3000 && infra_deriv[right] < 4)
    {
      setWall(posX, posY, (orientation / 2 + 1) % 4);
    }

    //balra van
    if (infra[left] < 3000 && infra[leftdi] < 5000 && pastinfra[left] < 3000 && infra_deriv[left] < 4)
    {
      setWall(posX, posY, (orientation / 2 + 3) % 4);
    }
  }
}

void setTurn(int16_t degree)
{
  int ratio = 360 / degree;
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

//---------------- STATES ---------------
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
    state = 'I';
    nextState = 'T';
  }
}
//Go until wall
void stateW()
{
  updatePosition();
  checkWalls();
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
