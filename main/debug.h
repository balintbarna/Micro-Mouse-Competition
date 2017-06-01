#define DEBUG 1
#if DEBUG
/* 0: semmi
   1: soros
   2: bt
   összegekkel több is megy egyszerre */
uint8_t debugMode = 3;
/* 0: nothing
   1: movement
   2: sensors
   4: states and params
   8: map
   16: floodfill map
   32: custom
   összegekkel több is megy egyszerre */
uint8_t outputMode = 23;
//Should the output include only data or title lines too
bool infoline = true;
//storage size for an output
const uint8_t labisize = mapsize * 2 + 1;
String labi[labisize];
//string constants
const String tab = "\t";
const String newline = "\r\n";
//Variable for serial output
String serialop = "";
//every X loop
#define loopNumber 50

//Function to display debug info on serial
void displayData()
{
  //Speed, Position, Time
  if (outputMode % 2 && !(overFloop % loopNumber))
  {
    if (infoline)
      serialop += "Speed, Position, Time" + newline;

    serialop += aggrSpeedLeft;
    serialop += tab;
    serialop += aggrSpeedRight;
    serialop += tab;
    serialop += encoderLeft.read();
    serialop += tab;
    serialop += encoderRight.read();
    serialop += tab;
    serialop += milli;
    serialop += newline;
  }

  //Infra sensors
  if ((outputMode >> 1) % 2 && !(overFloop % loopNumber))
  {
    if (infoline)
      serialop += "Infra sensors: left, leftdi, front, rightdi, right" + newline;

    serialop += infra[left];
    serialop += tab;
    serialop += infra[leftdi];
    serialop += tab;
    serialop += infra[front];
    serialop += tab;
    serialop += infra[rightdi];
    serialop += tab;
    serialop += infra[right];
    serialop += newline;
  }

  //States and params
  if ((outputMode >> 2) % 2 && !(overFloop % loopNumber))
  {
    if (infoline)
      serialop += "States, params, idler" + newline;

    serialop += state;
    serialop += tab;
    serialop += param1;
    serialop += tab;
    serialop += param2;
    serialop += tab;
    serialop += param3;
    serialop += tab;
    serialop += param4;
    serialop += tab;
    serialop += idler;
    serialop += newline;
  }

  //map
  if ((outputMode >> 3) % 2 && !(overFloop % loopNumber))
  {
    if (infoline)
      serialop += "Map" + newline;

    //init line holder
    String temp = "";
    //top line
    for (int i = 0; i < mapsize; i++)
      temp += " ---";
    labi[0] = temp;
    //for all rows
    for (int i = 0; i < mapsize; i++)
    {
      uint8_t realI = mapsize - 1 - i;
      temp = "";
      //for all columns
      for (int j = 0; j < mapsize; j++)
      {
        //If there's a wall to the left, add wall symbol
        if (getWall(j, realI, 3)) temp += "|";
        //else add space
        else temp += " ";
        //if that is current position add x marker
        if (posX == j && posY == realI) temp += " x ";
        //if that position has been visited add "-" marker
        else if (getVisited(j, realI)) temp += " - ";
        //else add spaces
        else temp += "   ";
      }
      //add line ending wall symbol
      temp += "|";
      //add line holder to array
      labi[2 * i + 1] = temp;
      //init line holder for horizontal walls
      temp = " ";
      //for all columns
      for (int j = 0; j < mapsize; j++)
      {
        //if there's a wall to the bottom, add wall symbol and space
        if (getWall(j, mapsize - 1 - i, 2)) temp += "--- ";
        //else add spaces
        else temp += "    ";
      }
      //add line holder to array
      labi[2 * (i + 1)] = temp;
    }
    //add array to output string
    serialop += newline;
    for (int i = 0; i < labisize; i++)
    {
      serialop += labi[i] + newline;
    }

    if (infoline)
      serialop += "Coordinates, orientation, saved coordinates and infra midzone" + newline;
    serialop += posX;
    serialop += tab;
    serialop += posY;
    serialop += tab;
    serialop += orientation;
    serialop += newline;
    serialop += savedPosX;
    serialop += tab;
    serialop += savedPosY;
    serialop += tab;
    serialop += infraMidZone;
    serialop += newline;
  }

  //floodfill map
  if ((outputMode >> 4) % 2 && !(overFloop % loopNumber))
  {
    if (infoline)
      serialop += "Floodfill Values Map + Path" + newline;

    String temp = "";
    for (int i = 0; i < mapsize; i++)
      temp += " ---";
    labi[0] = temp;
    for (int i = 0; i < mapsize; i++)
    {
      temp = "";
      for (int j = 0; j < mapsize; j++)
      {
        if (getWall(j, mapsize - 1 - i, 3)) temp += "|";
        else temp += " ";
        if (posX == j && posY == mapsize - 1 - i) temp += " x ";
        else
        {
          String t = cellValues[j][mapsize - 1 - i];
          while (t.length() < 3)
            t += " ";
          temp += t;
        }
      }
      temp += "|";
      labi[2 * i + 1] = temp;
      temp = " ";
      for (int j = 0; j < mapsize; j++)
      {
        if (getWall(j, mapsize - 1 - i, 2)) temp += "--- ";
        else temp += "    ";
      }
      labi[2 * (i + 1)] = temp;
    }
    serialop += newline;
    for (int i = 0; i < labisize; i++)
    {
      serialop += labi[i] + newline;
    }
    serialop += nextStep;
    serialop += newline;
    serialop += path;
    serialop += newline;
  }

  //custom
  if ((outputMode >> 5) % 2 && !(overFloop % loopNumber))
  {
    serialop += measuredTime;
  }


  //If anything to send, do so then reset String buffer
  if (serialop.length())
  {
    if (debugMode % 2)
    {
      Serial.println(serialop);
      Serial.send_now();
    }
    if ((debugMode >> 1) % 2)
      Serial3.println(serialop);
    serialop = "";
  }
}

//Function to simplify deconstructing a serial command
String getParam(int index, String comm)
{
  if (index == -1)
  {
    return comm;
  }
  else
  {
    return comm.substring(0, index);
  }
}

//Convert message recieved on serial to a command
void serialToValue() {
  bool newInfo = false;
  if (debugMode % 2)
  {
    if (Serial.available())
    {
      delayMicroseconds(100);
      serialCommand = Serial.readString();
      milli = 0;
      Serial.println(serialCommand);
      newInfo = true;
    }
  }
  if ((debugMode >> 1) % 2)
  {
    if (Serial3.available())
    {
      delayMicroseconds(100);
      serialCommand = Serial3.readString();
      milli = 0;
      Serial3.println(serialCommand);
      newInfo = true;
    }
  }
  //If we have new data
  if (newInfo)
  {
    //Read new state
    state = serialCommand[0];
    //Get first space location
    int spaceIndex = serialCommand.indexOf(' ');
    //If no space -> exit
    if (spaceIndex == -1) return;

    //Get string after space
    serialCommand = serialCommand.substring(spaceIndex + 1);
    //Get location of next space
    spaceIndex = serialCommand.indexOf(' ');
    //Get param
    param1 = getParam(spaceIndex, serialCommand).toInt();
    //If no space -> exit
    if (spaceIndex == -1) return;

    //Get string after space
    serialCommand = serialCommand.substring(spaceIndex + 1);
    //Get location of next space
    spaceIndex = serialCommand.indexOf(' ');
    //Get param
    param2 = getParam(spaceIndex, serialCommand).toInt();
    //If no space -> exit
    if (spaceIndex == -1) return;

    //Get string after space
    serialCommand = serialCommand.substring(spaceIndex + 1);
    //Get location of next space
    spaceIndex = serialCommand.indexOf(' ');
    //Get param
    param3 = getParam(spaceIndex, serialCommand).toInt();
    //If no space -> exit
    if (spaceIndex == -1) return;

    //Get string after space
    serialCommand = serialCommand.substring(spaceIndex + 1);
    //Get location of next space
    spaceIndex = serialCommand.indexOf(' ');
    //Get param
    param4 = getParam(spaceIndex, serialCommand).toInt();
    //If no space -> exit
    if (spaceIndex == -1) return;
  }
}
#endif
