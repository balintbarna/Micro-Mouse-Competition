//Function to modify orientation. Negative means left, 1 increment means 45°
#define cell_length 107
void turn(int8_t _size)
{
  orientation += _size;
  orientation += 8;
  orientation %= 8;
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
  if (abs(distance) < (cell_length / 8))
    midzone = true;
  else
    midzone = false;
}

