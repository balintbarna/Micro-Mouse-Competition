void PlanNextStep()
{
  CalculateBreathFirst();
  Coord calc;
  calc.x = pos.x;
  calc.y = pos.y;
  int calcOr = orientation;

  int8_t turn = getBestDirection(calc.x, calc.y) - calcOr;
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
  CalculateBreathFirst();
  path = "";
  Coord calc;
  calc.x = pos.x;
  calc.y = pos.y;
  int calcOr = orientation;

  while (cellValues[calc.x][calc.y])
  {
    int8_t turn = getBestDirection(calc.x, calc.y) - calcOr;
    turn += 8; turn %= 8;
    if (turn == 0)
    {
      path += "F";
      if (calcOr == 0)
        calc.y++;
      else if (calcOr == 2)
        calc.x++;
      else if (calcOr == 4)
        calc.y--;
      else if (calcOr == 6)
        calc.x--;
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
