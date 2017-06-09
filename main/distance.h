#define left 0
#define leftdi 1
#define front 2
#define rightdi 3
#define right 4
#define maxValueIndex 16
const uint8_t infraValueNumber = maxValueIndex + 1;

//Values array
const uint16_t analogInfraValues[][infraValueNumber] = {
  {57, 63, 286, 589, 716, 800, 851, 886, 914, 931, 946, 957, 963, 967, 970, 974, 976},
  {59, 69, 384, 614, 741, 817, 862, 891, 920, 936, 947, 956, 964, 969, 972, 976, 979},
  {59, 69, 384, 614, 741, 817, 862, 891, 920, 936, 947, 956, 964, 969, 972, 976, 979},
  {59, 69, 384, 614, 741, 817, 862, 891, 920, 936, 947, 956, 964, 969, 972, 976, 979},
  {58, 67, 420, 689, 782, 827, 884, 915, 938, 950, 962, 969, 974, 978, 981, 983, 985}
};

//Analog input pins
const int32_t inputs[] = {A3, A6, A2, A1, A0};
volatile int32_t _calib = 0;
void CalibrateInfra()
{
  _calib = 0;
  digitalWrite(infraPin, HIGH);
  delay(1);
  _calib = (1320 - (analogRead(inputs[left]) + analogRead(inputs[right]))) / 2;
  digitalWrite(infraPin, LOW);
}


//Read infra values, index: 0:left, 1:left-diagonal, 2:front, 3:right-diagonal, 4:right
void _readInfraPin(uint8_t index)
{
  //store last value
  pastinfra[index] = infra[index];
  //read analog value
  int16_t value = analogRead(inputs[index]) + _calib;
  //uint16_t value = analogRead(inputs[index]);
  if (value > analogInfraValues[index][maxValueIndex])
  {
    infra[index] = 999999;
  }
  else if (value < analogInfraValues[index][0])
  {
    infra[index] = 0;
  }
  else
  {
    for (uint8_t i = 0; i < maxValueIndex; i++)
    {
      int leftValue = analogInfraValues[index][i];
      int leftdistance = i * 500;
      int rightValue = analogInfraValues[index][i + 1];
      if (value == leftValue)
      {
        infra[index] = leftdistance;
        break;
      }
      else if (value < rightValue)
      {
        int fulldiff = rightValue - leftValue;
        int partdiff = value - leftValue;
        int adddist = partdiff * 500 / fulldiff;
        int distance = leftdistance + adddist;

        infra[index] = distance;
        break;
      }
    }
  }
  infra_deriv[index] = 1000 * (infra[index] - pastinfra[index]) / (int)micro;
}

void _readInfra(int8_t index = -1)
{
  if (index == -1)
  {
    for (uint8_t i = 0; i < 5; i++)
    {
      _readInfraPin(i);
    }
  }
  else
  {
    _readInfraPin(index);
  }
}

volatile uint8_t _infraCounter = 0;
void InfraISR()
{
  if (_infraCounter == 0)
  {
    digitalWrite(infraPin, 1);
    _infraCounter = 1;
    return;
  }
  if (_infraCounter == 1)
  {
    _readInfra();
    digitalWrite(infraPin, 0);
    micro = 0;
    _infraCounter = 0;
    return;
  }
}

//#include <VL53L0X.h>
//VL53L0X TOF;
//
////Function to setup TOF sensor for reading distance
//void SetupTOF()
//{
//  TOF.init();
//  TOF.setTimeout(50);
//  TOF.setMeasurementTimingBudget(20000);
//}
//
//int32_t readTOF()
//{
//  int temp = TOF.readRangeSingleMillimeters();
//  if (TOF.timeoutOccurred())
//  {
//    temp = 999999;
//  }
//  return temp;
//}

uint8_t getSideWalls()
{
  //Jobb fal vizsgálata
  uint8_t side_walls = infra[right] < sideInfraLimit && infra[rightdi] < diagonalInfraLimit && pastinfra[right] < sideInfraLimit && infra_deriv[right] < derivInfraLimit;
  //Bal fal vizsgálata
  side_walls += (infra[left] < sideInfraLimit && infra[leftdi] < diagonalInfraLimit && pastinfra[left] < sideInfraLimit && infra_deriv[left] < derivInfraLimit) << 1;
  return side_walls;
}


