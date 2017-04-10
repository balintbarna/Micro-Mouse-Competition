void stateC()
{
  CascadePos(param1, param2);
}

void stateS()
{
  SetMotorSpeed(0, 0);
  //SetMotorPower(0, 0);
}

void stateV()
{
  SetMotorSpeed(param1, param2);
}

void stateT()
{
  int de = (1650 - s4) / 100;
  SetMotorSpeed(300 - de, 300 + de);
  if (s2 < 9000) state = 'S';
}

