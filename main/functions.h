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
  leftPosOld = 0;
  rightPosOld = 0;
  for (int i = 0; i < 5; i++)
  {
    infra_deriv[i] = 0;
  }
}

