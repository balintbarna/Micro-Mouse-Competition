/*   xWalls       y
  --- --- --- ---
                  2
  --- ---  |  ---
                  1
  --- --- --- ---
                  0
  --- --- --- ---
  x0   1   2   3
   itt a kijelölt fal elem pl 2,1,0 vagy 2,2,2

  ----------------------

      yWalls      y
  |   |   |   |   2
  |   |   -   |   1
  |   |   |   |   0

  x 0   1   2   3
  itt a kijelölt fal elem pl 1,1,1 vagy 2,1,3

*/

int32_t xWalls[31] = {
  0, 0, 0, 0, 0,
  0, 0, 0, 0, 0,
  0, 0, 0, 0, 0,
  0, 0, 0, 0, 0,
  0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0
};

int32_t yWalls[31] = {
  0, 0, 0, 0, 0,
  0, 0, 0, 0, 0,
  0, 0, 0, 0, 0,
  0, 0, 0, 0, 0,
  0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0
};


/* which_wall values
    0 means x higher
    1 means y higher
    2 means x lower
    3 means y lower

      0
   3     1
      2
*/
int8_t getWall(int8_t x, int8_t y, uint8_t which_wall)
{
  //yWalls
  if (which_wall % 2)
  {
    //lower (left)
    if (which_wall - 1)
    {
      if (x == 0)return 1;
      return (yWalls[x - 1] >> y) % 2;
    }
    //higher (right)
    else
    {
      if (x == mapsize - 1)return 1;
      return (yWalls[x] >> y) % 2;
    }
  }
  //xWalls
  else
  {
    //lower (bottom)
    if (which_wall)
    {
      if (y == 0)return 1;
      return (xWalls[y - 1] >> x) % 2;
    }
    //higher (top)
    else
    {
      if (y == mapsize - 1)return 1;
      return (xWalls[y] >> x) % 2;
    }
  }
}
void setWall(int8_t x, int8_t y, uint8_t which_wall)
{
  //yWalls
  if (which_wall % 2)
  {
    //lower
    if (which_wall - 1)
    {
      if (x == 0)return;
      yWalls[x - 1] |= (1 << y);
    }
    //higher
    else
    {
      if (x == mapsize - 1)return;
      yWalls[x] |= (1 << y);
    }
  }
  //xWalls
  else
  {
    //lower
    if (which_wall)
    {
      if (y == 0)return;
      xWalls[y - 1] |= (1 << x);
    }
    //higher
    else
    {
      if (y == mapsize - 1)return;
      xWalls[y] |= (1 << x);
    }
  }
}

void checkWalls()
{
  //Szembe van
  if (infra[front] < (1500 + maxSpeed))
    setWall(posX, posY, (orientation / 2) % 4);

  if (midzone)
  {
    //jobbra van
    if (infra[right] < 3500 && infra[rightdi] < 6000 && pastinfra[right] < 3500)
    {
      setWall(posX, posY, (orientation / 2 + 1) % 4);
    }

    //balra van
    if (infra[left] < 3500 && infra[leftdi] < 6000 && pastinfra[left] < 3500)
    {
      setWall(posX, posY, (orientation / 2 + 3) % 4);
    }
  }
}
