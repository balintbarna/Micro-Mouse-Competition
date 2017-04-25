/*   xWalls     y
  -- -- -- --   2
  -- -- || --   1
  -- -- -- --   0
  -- -- -- --   -
  -2 -1  0  1   x

  ----------------------

      yWalls      y
  |   |   |   |   1
  |   |   -   |   0
  |   |   |   |   -1

  -   0   1   2   x

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


/* which values
    0 means x higher
    1 means y higher
    2 means x lower
    3 means y lower

      0
   3     1
      2
*/
int8_t getWall(int8_t x, int8_t y, int8_t which_wall)
{
  //y
  if (which_wall % 2)
  {
    //lower
    if (which_wall - 1)
    {
      if (y == 0)return 1;
      return (yWalls[x - 1] >> y) % 2;
    }
    //higher
    else
    {
      if (y == mapsize - 1)return 1;
      return (yWalls[x] >> x) % 2;
    }
  }
  //x
  else
  {
    //lower
    if (which_wall)
    {
      if (x == 0)return 1;
      return (xWalls[y - 1] >> x) % 2;
    }
    //higher
    else
    {
      if (x == mapsize - 1)return 1;
      return (xWalls[y] >> x) % 2;
    }
  }
}
void setWall(int8_t x, int8_t y, int8_t which_wall)
{
  //y
  if (which_wall % 2)
  {
    //lower
    if (which_wall - 1)
    {
      if (y == 0)return;
      yWalls[x - 1] |= (1 << y);
    }
    //higher
    else
    {
      if (y == mapsize - 1)return;
      yWalls[x] |= (1 << x);
    }
  }
  //x
  else
  {
    //lower
    if (which_wall)
    {
      if (x == 0)return;
      xWalls[y - 1] |= (1 << x);
    }
    //higher
    else
    {
      if (x == mapsize - 1)return;
      xWalls[y] |= (1 << x);
    }
  }
}

void checkWalls()
{
  //Szembe van
  if (infra[front] < 5500)
    setWall(posX, posY, 0);

  //jobbra van
  if (infra[right] < 4500 && infra[rightdi] < 6000 && pastinfra[right] < 4500 && infra_deriv[right] < 100000)
  {
    setWall(posX, posY, 1);
  }

  //balra van
  if (infra[left] < 4500 && infra[leftdi] < 6000 && pastinfra[left] < 4500 && infra_deriv[left] < 100000)
  {
    setWall(posX, posY, 3);
  }
}

