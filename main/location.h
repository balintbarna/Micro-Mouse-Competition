#include "libs/mpu6050.cpp"
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
  while(!mpuready());
  baseYaw=mpuread();
}

