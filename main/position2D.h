//Function to modify orientation. Negative means left, 1 increment means 45°
void turn(int8_t _size)
{
  orientation += _size;
  orientation += 3;
  orientation %= 8;
  orientation -= 3;
}

void updatePosition()
{
  int temp = (encoderLeft.read() + encoderRight.read()) / 2;
  //alapirany
  if (orientation == 0)
  {
    posY = lastPosY + temp / 107;
    if (abs(temp % 107) < 30)
      midzone = true;
    else
      midzone = false;
  }
  //hátra
  else if (orientation == 4)
  {
    posY = lastPosY - temp / 107;
    if (abs(temp % 107) < 30)
      midzone = true;
    else
      midzone = false;
  }
  //jobbra
  else if (orientation == 2)
  {
    posX = lastPosX - temp / 107;
    if (abs(temp % 107) < 30)
      midzone = true;
    else
      midzone = false;
  }
  //balra
  else if (orientation == -2)
  {
    posX = lastPosX - temp / 107;
    if (abs(temp % 107) < 10)
      midzone = true;
    else
      midzone = false;
  }
}

