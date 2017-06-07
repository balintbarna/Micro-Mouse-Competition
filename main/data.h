#include <EEPROM.h>
QueueList<Coord>queue;

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
uint8_t getWall(uint8_t x, uint8_t y, uint8_t which_wall)
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
void setWall(uint8_t x, uint8_t y, uint8_t which_wall)
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

void setVisited(uint8_t x, uint8_t y)
{
  visited[x] |= 1 << y;
}

uint8_t getVisited(uint8_t x, uint8_t y)
{
  return (visited[x] >> y) % 2;
}

void checkWalls()
{
  //Szembe van
  if (infra[front] < (frontInfraLimit + breakLengthInfra))
    setWall(pos.x, pos.y, (orientation / 2) % 4);

  if (infraMidZone)
  {
    //jobbra van
    if (infra[right] < 2500)
    {
      setWall(pos.x, pos.y, (orientation / 2 + 1) % 4);
    }

    //balra van
    if (infra[left] < 2500)
    {
      setWall(pos.x, pos.y, (orientation / 2 + 3) % 4);
    }
  }
}

void clearAllData()
{

}

int8_t getBestDirectionOpt(int8_t x, int8_t y, int8_t ori)
{
  uint8_t counter = 0;
  while (true)
  {
    if (ori == 0)
    {
      //not top and no wall to the top
      if (y < (mapsize - 1) && !getWall(x, y, 0))
      {
        //check cell to the top
        if (cellValues[x][y + 1] < cellValueMax)
        {
          return ori;
        }
      }
    }
    if (ori == 2)
    {
      //not rightmost and no wall to the right
      if (x < (mapsize - 1) && !getWall(x, y, 1))
      {
        //check cell to the right
        if (cellValues[x + 1][y] < cellValueMax)
        {
          return ori;
        }
      }
    }
    if (ori == 4)
    {
      //not bottom and no wall to the bottom
      if (y && !getWall(x, y, 2))
      {
        //check cell to the bottom
        if (cellValues[x][y - 1] < cellValueMax)
        {
          return ori;
        }
      }
    }
    if (ori == 6)
    {
      //not leftmost and no wall to the left
      if (x && !getWall(x, y, 3))
      {
        //check cell to the left
        if (cellValues[x - 1][y] < cellValueMax)
        {
          return ori;
        }
      }
    }
    if (counter == 0)
      ori += 2;
    else if (counter == 1)
      ori += 4;
    else if (counter == 2)
      ori += 6;
    else
    {
      ori += 4;
      ori %= 8;
      return ori;
    }

    ori %= 8;
    counter++;
  }
}


int8_t getBestDirection(int8_t x, int8_t y)
{
  uint16_t lowest = cellValueMax;
  int8_t ori = 0;
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
  if (x < (mapsize - 1) && !getWall(x, y, 1))
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
  if (y < (mapsize - 1) && !getWall(x, y, 0))
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

uint16_t getLowestNeighbour(int8_t x, int8_t y)
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

void SetupFloodfill()
{
  for (int i = 0; i < mapsize; i++)
  {
    for (int j = 0; j < mapsize; j++)
    {
      cellValues[i][j] = cellValueMax;
    }
  }
  cellValues[goal.x][goal.y] = 0;
}
/*
  void CalculateFloodfill()
  {
  SetupFloodfill();
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
*/
void CalculateBreathFirst()
{
  while (!queue.isEmpty())
    queue.pop();
  SetupFloodfill();
  queue.push(goal);
  while (!queue.isEmpty())
  {
    Coord cell = queue.pop();
    if (cell.x == pos.x && cell.y == pos.y)
    {
      return;
    }
    uint16_t cellvalue = cellValues[cell.x][cell.y];
    //not leftmost and no wall to the left
    if (cell.x && !getWall(cell.x, cell.y, 3))
    {
      if (cell.x - 1 == pos.x && cell.y == pos.y)
      {
        return;
      }
      //check if value is higher than own+1
      if (cellValues[cell.x - 1][cell.y] > (cellvalue + 1))
      {
        //set value to own+1
        cellValues[cell.x - 1][cell.y] = cellvalue + 1;
        //add to queue
        queue.push({cell.x - 1, cell.y});
      }
    }
    //not rightmost and no wall to the right
    if (cell.x - mapsize + 1 && !getWall(cell.x, cell.y, 1))
    {
      if (cell.x + 1 == pos.x && cell.y == pos.y)
      {
        return;
      }
      //check if value is higher than own+1
      if (cellValues[cell.x + 1][cell.y] > (cellvalue + 1))
      {
        //set value to own+1
        cellValues[cell.x + 1][cell.y] = cellvalue + 1;
        //add to queue
        queue.push({cell.x + 1, cell.y});
      }
    }
    //not bottom and no wall to the bottom
    if (cell.y && !getWall(cell.x, cell.y, 2))
    {
      if (cell.x == pos.x && cell.y - 1 == pos.y)
      {
        return;
      }
      //check if value is higher than own+1
      if (cellValues[cell.x][cell.y - 1] > (cellvalue + 1))
      {
        //set value to own+1
        cellValues[cell.x][cell.y - 1] = cellvalue + 1;
        //add to queue
        queue.push({cell.x, cell.y - 1});
      }
    }
    //not top and no wall to the top
    if (cell.y - mapsize + 1 && !getWall(cell.x, cell.y, 0))
    {
      if (cell.x == pos.x && cell.y + 1 == pos.y)
      {
        return;
      }
      //check if value is higher than own+1
      if (cellValues[cell.x][cell.y + 1] > (cellvalue + 1))
      {
        //set value to own+1
        cellValues[cell.x][cell.y + 1] = cellvalue + 1;
        //add to queue
        queue.push({cell.x, cell.y + 1});
      }
    }
  }
}

