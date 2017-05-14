#include "libs/mpu6050.h"
int16_t baseYaw = 0;
volatile int16_t relativeTurnError = 0;

//Variables for position in a 2D matrix
//0;0 a kiindulási pont
volatile int8_t posX = 0;
volatile int8_t posY = 0;
volatile int8_t savedPosX = 0;
volatile int8_t savedPosY = 0;
volatile int32_t lastPosEncAvg = 0;
//A cella közepéről induljon
bool midzone = true;

//0 az előre, -3től 4-ig irányt mutat
/*    7  0  1
      6     2
      5  4  3
*/
volatile uint8_t orientation = 0;

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
#define cell_length 321
void turn(int8_t _size)
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
  //Ha merőleges irányok
  if (!(orientation % 2))
  {
    //Elmentett pozíció, ha megtett egy merőleges cella hosszt
    if (abs(distance) >= cell_length)
    {
      //Ha X irányú
      if (orientation % 4)
      {
        //jobbra (poz)
        if (orientation == 2)
        {
          savedPosX += sign(distance);
        }
        //balra (neg)
        else
        {
          savedPosX -= sign(distance);
        }
      }
      //Y irányú
      else
      {
        //lefelé (neg)
        if (orientation)
        {
          savedPosY -= sign(distance);
        }
        //alapirány (poz)
        else
        {
          savedPosY += sign(distance);
        }
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
        //jobbra (poz)
        if (orientation == 2)
        {
          posX = savedPosX + sign(distance);
        }
        //balra (neg)
        else
        {
          posX = savedPosX - sign(distance);
        }
        posY = savedPosY;
      }
      //Y irányú
      else
      {
        //lefelé (neg)
        if (orientation)
        {
          posY = savedPosY - sign(distance);
        }
        //alapirány (poz)
        else
        {
          posY = savedPosY + sign(distance);
        }
        posX = savedPosX;
      }
    }
    //Ha még ott van
    else
    {
      posX = savedPosX;
      posY = savedPosY;
    }
  }
  int newdistance = (posX - savedPosX + posY - savedPosY) * cell_length - distance;
  if (abs(newdistance) < (cell_length / 7))
    midzone = true;
  else
    midzone = false;
}

void ResetLocation()
{
  lastPosEncAvg = 0;
  savedPosX = posX;
  savedPosY = posY;
}

