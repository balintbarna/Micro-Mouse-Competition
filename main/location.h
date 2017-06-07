#include "libs/mpu6050.h"
int16_t baseYaw = 0;
volatile int16_t relativeTurnError = 0;

void readTurnError()
{
  while (mpuready())
  {
    int16_t temp = mpuread();
    //Correction by zero value
    temp -= baseYaw;
    //Correction by expected direction
    temp -= orientation * 45;
    //Normalizing value
    temp %= 360;
    //Getting between -179° to 180°
    if (temp > 180)
    {
      temp -= 360;
    }
    else if (temp <= -180)
    {
      temp += 360;
    }
    relativeTurnError = temp;
  }
}

void setYawCorrection()
{
  while (!mpuready());
  baseYaw = mpuread();
}

//Function to modify orientation. Negative means left, 1 increment means 45°
#define cell_length 318
void setOrientation(int8_t _size)
{
  orientation += _size;
  orientation += 8;
  orientation %= 8;
}

//Function to return sign of any type
template <typename type>
type sign(type value) {
  return type((value > 0) - (value < 0));
}

void updatePosition()
{
  int posEncAvg = (encoderLeft.read() + encoderRight.read()) / 2;
  int distance = posEncAvg - lastPosEncAvg;
  //savedPos frissítése
  //Ha merőleges irányok
  if (!(orientation % 2))
  {
    //Elmentett pozíció, ha megtett egy merőleges cella hosszt
    if (abs(distance) >= cell_length)
    {
      //Ha X irányú
      if (orientation % 4)
      {
        int adder = -(orientation / 2 - 2) * sign(distance);
        savedPos.x += adder;
      }
      //Y irányú
      else
      {
        int adder = -(orientation / 2 - 1) * sign(distance);
        savedPos.y += adder;
      }
      lastPosEncAvg = posEncAvg;
      distance = 0;
    }
    //Valós pozíció, ha messze van az elmentetttől
    if (abs(distance) >= cell_length / 2)
    {
      //Ha X irányú
      if (orientation % 4)
      {
        int adder = -(orientation / 2 - 2) * sign(distance);
        pos.x = savedPos.x + adder;
      }
      //Y irányú
      else
      {
        int adder = -(orientation / 2 - 1) * sign(distance);
        pos.y = savedPos.y + adder;
      }
    }
    //Ha még ott van
    else
    {
      pos.y = savedPos.x;
      pos.y = savedPos.y;
    }
  }

  int cellMidDistance = (abs(pos.x - savedPos.x) + abs(pos.y - savedPos.y)) * cell_length - distance;
  if (abs(cellMidDistance) < (cell_length / 3))
    cellMidZone = true;
  else
    cellMidZone = false;

  int infraMidDistance = cellMidDistance - 135;
  if (abs(infraMidDistance) < (cell_length / 40))
    infraMidZone = true;
  else
    infraMidZone = false;
  setVisited(pos.x, pos.y);
}

void ResetLocation()
{
  lastPosEncAvg = 0;
  savedPos.x = pos.x;
  savedPos.y = pos.y;
}

