#include "libs/mpu6050.cpp"
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
  while(!mpuready());
  baseYaw=mpuread();
}

