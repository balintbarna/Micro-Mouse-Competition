//Function to remap floating point values
float mapfloat(float x, float in_min, float in_max, float out_min, float out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

//Function to return sign of any type
template <typename type>
type sign(type value) {
  return type((value > 0) - (value < 0));
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

//Function to erase past stored values
void ResetAllStoredValues()
{
  ResetEncoders();
  aggrSpeedLeft = 0;
  aggrSpeedRight = 0;
  errSumLeft = 0;
  errSumRight = 0;
//  pastSetSpeedLeft = 0;
//  pastSetSpeedRight = 0;
  leftPosOld = 0;
  rightPosOld = 0;
}

