#if DEBUG
String tab = "\t";
String newline = "\n";
const int labisize = mapsize * 2 + 1;
String labi[labisize];
void serialToValue() {
  bool newInfo = false;
  if (debugMode % 2)
  {
    if (Serial.available())
    {
      delayMicroseconds(500);
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
      delayMicroseconds(500);
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
  if (outputMode % 2 && !(overFloop % 300))
  {
    serialop += aggrSpeedLeft;
    serialop += tab;
    serialop += aggrSpeedRight;
    serialop += tab;
    serialop += encoderLeft.read();
    serialop += tab;
    serialop += encoderRight.read();
    serialop += tab;
    serialop += elapsedTime;
    serialop += tab;
  }

  //Infra sensors
  if ((outputMode >> 1) % 2)
  {
    serialop += infra[left];
    serialop += tab;
    serialop += infra[leftdi];
    serialop += tab;
    serialop += infra[front];
    serialop += tab;
    serialop += infra[rightdi];
    serialop += tab;
    serialop += infra[right];
    serialop += tab;
  }

  //States and params
  if ((outputMode >> 2) % 2)
  {
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
    serialop += tab;
  }
  
  //position
  if ((outputMode >> 3) % 2 && !(overFloop % 250))
  {
    String temp = "";
    for (int i = 0; i < mapsize; i++)
      temp += " ---";
    labi[0] = temp;
    for (int i = 0; i < mapsize; i++)
    {
      temp = "";
      for (int j = 0; j < mapsize; j++)
      {
        if (getWall(j, mapsize - 1 - i, 3)) temp += "| ";
        else temp += "  ";
        if (posX == j && posY == mapsize - 1 - i) temp += "x ";
        else temp += "  ";
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
    serialop += posX;
    serialop += tab;
    serialop += posY;
    serialop += tab;
    serialop += orientation;
    serialop += tab;
  }


  //If anything to send, do so then reset String buffer
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
