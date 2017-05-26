

void PlanPathToTarget()
{
  path = "";
  int calcX = posX;
  int calcY = posY;
  int calcOr = orientation;

  while (!cellValues[calcX][calcY])
  {
    uint8_t turn = getBestDirection(calcX, calcY) - calcOr; turn += 8; turn %= 8;
    if (turn == 0)
    {
      path += "F";
      if (calcOr == 0)
        calcX++;
      else if (calcOr == 2)
        calcY++;
      else if (calcOr == 4)
        calcX--;
      else if (calcOr == 6)
        calcY--;
    }
    else
    { if (turn == 2)
      {
        path += "R";
      }
      else if (turn == 4)
      {
        path += "RR";
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


//  if (midzone)
//  {
//    //célba jutott
//    if (!cellValues[posX][posY])
//      state = 'S';
//    //még nem jutott célba
//    else
//    {
//      if (computed)
//      {
//        int temp = getBestDirection(posX, posY);
//        int temp2 = temp - orientation;
//        if (!temp2)
//        {
//          ResetAllStoredValues();
//          SetMotorPower(0, 0);
//          setTurn(temp2 * 45);
//        }
//      }
//    }
//  }
}
