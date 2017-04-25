//Function to modify orientation. Negative means left, 1 increment means 45°
int32_t lastPosEncAvg = 0;
#define cell_length 104
void turn(int8_t _size)
{
  orientation += _size;
  orientation += 8;
  orientation %= 8;
}

void updatePosition()
{
  int posEncAvg = (encoderLeft.read() + encoderRight.read()) / 2;
  int temp = posEncAvg - lastPosEncAvg;
  //Ha merőleges irányok
  if (!(orientation % 2))
  {
    //Ha megtett egy merőleges cella hosszt
    if (temp >= cell_length)
    {
      //Ha alapirány
      if (orientation == 0)
      {
        posY++;
      }
      //hátra
      else if (orientation == 4)
      {
        posY--;
      }
      //jobbra
      else if (orientation == 2)
      {
        posX++;
      }
      //balra
      else if (orientation == 6)
      {
        posX--;
      }
      lastPosEncAvg = posEncAvg;
    }
  }
  if (abs(temp) < cell_length/4)
    midzone = true;
  else
    midzone = false;
}

