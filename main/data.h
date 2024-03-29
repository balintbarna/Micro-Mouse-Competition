#include <EEPROM.h>
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
//store vertical walls
volatile uint32_t xWalls[mapsize - 1];
//store horizontal walls
volatile uint32_t yWalls[mapsize - 1];
//store which cells have been visited
volatile uint32_t visited[mapsize];
//store values of cells for maze solving
volatile uint16_t cellValues[mapsize][mapsize];

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

//Store 32 bit to EEPROM
void Store32 (uint16_t address, uint32_t value)
{
  for (int i = 0; i < 4; i++)
  {
    EEPROM.write(i + address * 4, value);
    value >>= 8;
  }
}

uint32_t Read32(uint16_t address)
{
  uint32_t readval = 0;
  for (uint8_t i = 3; i >= 0; i--)
  {
    readval = readval << 8;
    readval += EEPROM.read(i + 4 * address);
  }
  return readval;
}
void StoreArray32(uint16_t address, uint32_t* data)
{
  uint8_t datasize = sizeof(data) / 4;
  for (uint8_t i = 0; i < datasize; i++ )
  {
    Store32(address + i, data[i]);
  }
}
void ReadArray32(uint16_t address, uint32_t* data)
{
  uint8_t datasize = sizeof(data) / 4;
  for (uint8_t i = 0; i < datasize; i++)
  {
    data[i] = Read32(address + i);
  }
}

void setVisited(int8_t x, int8_t y)
{
  visited[x] |= 1 << y;
}

uint8_t getVisited(int8_t x, int8_t y)
{
  return (visited[x] >> y) % 2;
}

void checkWalls()
{
  //Szembe van
  if (infra[front] < (frontInfraLimit + breakLengthInfra))
    setWall(posX, posY, (orientation / 2) % 4);

  if (midzone)
  {
    uint8_t side_walls = getSideWalls(false);
    //jobbra van
    if (infra[right] < 3500)
    {
      setWall(posX, posY, (orientation / 2 + 1) % 4);
    }

    //balra van
    if (infra[left] < 3500)
    {
      setWall(posX, posY, (orientation / 2 + 3) % 4);
    }
  }
}

void clearAllData()
{

}

uint8_t getBestDirection(int8_t x, int8_t y)
{
  uint16_t lowest = cellValueMax;
  uint8_t ori = 0;
  //not leftmost and no wall to the left
  if (x && !getWall(x, y, 3))
  {
    //check cell to the left
    int temp = cellValues[x - 1][y];
    if (temp < lowest)
    {
      lowest = temp;
      ori = 6;
    }
  }
  //not rightmost and no wall to the right
  if (x - mapsize + 1 && !getWall(x, y, 1))
  {
    //check cell to the right
    int temp = cellValues[x + 1][y];
    if (temp < lowest)
    {
      lowest = temp;
      ori = 2;
    }
  }
  //not bottom and no wall to the bottom
  if (y && !getWall(x, y, 2))
  {
    //check cell to the bottom
    int temp = cellValues[x][y - 1];
    if (temp < lowest)
    {
      lowest = temp;
      ori = 4;
    }
  }
  //not top and no wall to the top
  if (y - mapsize + 1 && !getWall(x, y, 0))
  {
    //check cell to the top
    int temp = cellValues[x][y + 1];
    if (temp < lowest)
    {
      lowest = temp;
      ori = 0;
    }
  }
  return ori;
}

uint32_t getLowestNeighbour(int8_t x, int8_t y)
{
  uint32_t lowest = cellValueMax;
  //not leftmost and no wall to the left
  if (x && !getWall(x, y, 3))
  {
    lowest = cellValues[x - 1][y];
  }
  //not rightmost and no wall to the right
  if (x - mapsize + 1 && !getWall(x, y, 1))
  {
    int temp = cellValues[x + 1][y];
    lowest = temp < lowest ? temp : lowest;
  }
  //not bottom and no wall to the bottom
  if (y && !getWall(x, y, 2))
  {
    int temp = cellValues[x][y - 1];
    lowest = temp < lowest ? temp : lowest;
  }
  //not top and no wall to the top
  if (y - mapsize + 1 && !getWall(x, y, 0))
  {
    int temp = cellValues[x][y + 1];
    lowest = temp < lowest ? temp : lowest;
  }
  return lowest;
}

void SetupMazeSolver()
{
  for (int i = 0; i < mapsize; i++)
  {
    for (int j = 0; j < mapsize; j++)
    {
      cellValues[i][j] = cellValueMax;
    }
  }
  cellValues[goalX][goalY] = 0;
}

void SolveMaze()
{
  SetupMazeSolver();
  bool changed = true;
  while (changed)
  {
    changed = false;
    for (int i = 0; i < mapsize; i++)
    {
      for (int j = 0; j < mapsize; j++)
      {
        uint32_t temp = getLowestNeighbour(i, j) + 1;
        if (cellValues[i][j] > temp)
        {
          cellValues[i][j] = temp;
          changed = true;
        }
      }
    }
  }
}

