#if DEBUG
void serialToValue() {
  bool newInfo = false;
  if (debugMode % 2)
  {
    if (Serial.available())
    {
      delay(1);
      serialCommand = Serial.readString();
      elapsedTime = 0;
      Serial.println(serialCommand);
      newInfo = true;
    }
  }
  if ((debugMode >> 1) % 2)
  {
    if (Serial3.available())
    {
      delay(1);
      serialCommand = Serial3.readString();
      elapsedTime = 0;
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

void displayData()
{
  //Speed, Position, Time
  if (outputMode % 2)
  {
    serialop += aggrSpeedLeft;
    serialop += "\t";
    serialop += aggrSpeedRight;
    serialop += "\t";
    serialop += encoderLeft.read();
    serialop += "\t";
    serialop += encoderRight.read();
    serialop += "\t";
    serialop += elapsedTime;
    serialop += "\t";
  }

  //Infra sensors
  if ((outputMode >> 1) % 2)
  {
    serialop += infra[left];
    serialop += "\t";
    serialop += infra[leftdi];
    serialop += "\t";
    serialop += infra[front];
    serialop += "\t";
    serialop += infra[rightdi];
    serialop += "\t";
    serialop += infra[right];
    serialop += "\t";
  }

  //States and params
  if ((outputMode >> 2) % 2)
  {
    serialop += state;
    serialop += "\t";
    serialop += param1;
    serialop += "\t";
    serialop += param2;
    serialop += "\t";
    serialop += param3;
    serialop += "\t";
    serialop += param4;
    serialop += "\t";
    serialop += idler;
    serialop += "\t";
  }

  //Map
  if ((outputMode >> 3) % 2)
  {
    //every x-th loop
    int repeatafter = 5000;
    if (!(overFloop % repeatafter))
    {
      //topmost row
      for (int i = 0; i < mapsize; i++)
        serialop += " _";
    }
    if (!((overFloop - 1) % repeatafter))
    {
      //All middle rows
      for (int i = 0; i < mapsize - 1; i++)
      {
        //yWalls
        //beginning of row
        serialop += "| ";
        //all middle walls
        for (int j = 0; j < mapsize - 1; j++)
        {
          if ((yWalls[j] >> (mapsize - 2 - i)) % 2)
          {
            serialop += "| ";
          }
          else
          {
            serialop += "  ";
          }
        }
        //end of row
        serialop += "|\n";
        //xWalls
        for (int j = 0; j < mapsize - 1; j++)
        {
          if (( xWalls[mapsize - 1 - i] >> j) % 2)
          {
            serialop += " -";
          }
          else
          {
            serialop += "  ";
          }
        }
        serialop += "\n";
      }
    }

    if (!((overFloop - 2) % repeatafter))
    {

      //last yWall row
      //beginning of row
      serialop += "| ";
      //all middle walls
      for (int j = 0; j < mapsize - 1; j++)
      {
        if (yWalls[j] % 2)
        {
          serialop += "| ";
        }
        else
        {
          serialop += "  ";
        }
      }

      //end of row
      serialop += "|";
    }
    if (!((overFloop - 3) % repeatafter))
    {

      //last row
      for (int i = 0; i < mapsize; i++)
        serialop += " T";
    }
  }

  if (serialop.length())
  {
    if (debugMode % 2)
      Serial.println(serialop);
    if ((debugMode >> 1) % 2)
      Serial3.println(serialop);
    serialop = "";
  }

}
#endif
