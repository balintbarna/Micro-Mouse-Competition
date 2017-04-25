#include <VL53L0X.h>
VL53L0X TOF;

//Function to setup TOF sensor for reading distance
void SetupTOF()
{
  TOF.init();
  TOF.setTimeout(50);
  TOF.setMeasurementTimingBudget(20000);
}

int32_t readTOF()
{
  int temp = TOF.readRangeSingleMillimeters() * 100;
  if (TOF.timeoutOccurred())
  {
    temp = 999999;
  }
  return temp;
}
