void PlanNextStep()
{
  CalculateFloodfill();
  int calcX = posX;
  int calcY = posY;
  int calcOr = orientation;

  int8_t turn = getBestDirection(calcX, calcY) - calcOr;
  turn += 8; turn %= 8;

  if (turn == 0)
  {
    nextStep = 'F';
  }
  else if (turn == 2)
  {
    nextStep = 'R';
  }
  else if (turn == 4)
  {
    nextStep = 'B';
  }
  else if (turn == 6)
  {
    nextStep = 'L';
  }
}

void PlanPathToTarget()
{
  CalculateFloodfill();
  path = "";
  int calcX = posX;
  int calcY = posY;
  int calcOr = orientation;

  while (cellValues[calcX][calcY])
  {
    int8_t turn = getBestDirection(calcX, calcY) - calcOr;
    turn += 8; turn %= 8;
    if (turn == 0)
    {
      path += "F";
      if (calcOr == 0)
        calcY++;
      else if (calcOr == 2)
        calcX++;
      else if (calcOr == 4)
        calcY--;
      else if (calcOr == 6)
        calcX--;
    }
    else
    {
      if (turn == 2)
      {
        path += "R";
      }
      else if (turn == 4)
      {
        path += "B";
      }
      else if (turn == 6)
      {
        path += "L";
      }
      calcOr += turn;
      calcOr += 8;
      calcOr %= 8;
    }
  }
}
