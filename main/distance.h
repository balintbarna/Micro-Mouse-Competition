#define left 0
#define leftdi 1
#define front 2
#define rightdi 3
#define right 4
#define maxValueIndex 16
const uint8_t infraValueNumber = maxValueIndex + 1;

//Values array
const int16_t analogInfraValues[][infraValueNumber] = {
  {58, 64, 302, 593, 735, 816, 865, 895, 916, 934, 946, 956, 964, 970, 975, 978, 980},
  {58, 64, 302, 593, 735, 816, 865, 895, 916, 934, 946, 956, 964, 970, 975, 978, 990},
  {57, 66, 228, 510, 684, 771, 822, 857, 883, 905, 922, 933, 942, 950, 956, 961, 965},
  {58, 64, 302, 593, 735, 816, 865, 895, 916, 934, 946, 956, 964, 970, 975, 978, 990},
  {57, 64, 230, 561, 730, 815, 873, 904, 925, 940, 952, 960, 965, 971, 975, 977, 978}
};

//Analog input pins
const int32_t inputs[] = {A3, A6, A2, A1, A0};
volatile int32_t _calib = 0;
void _calibrateInfra()
{
  _calib = 0;
  digitalWrite(infraPin, HIGH);
  delay(1);
  _calib = (1400 - (analogRead(inputs[left]) + analogRead(inputs[right]))) / 2;
  digitalWrite(infraPin, LOW);
}


//Read infra values, index: 0:left, 1:left-diagonal, 2:front, 3:right-diagonal, 4:right
void _readInfraPin(uint8_t index)
{
  //store last value
  pastinfra[index] = infra[index];
  //read analog value
  //int32_t value = analogRead(inputs[index]) + _calib;
  uint16_t value = analogRead(inputs[index]);
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

uint8_t getSideWalls(bool with_deriv = true)
{
  //Jobb fal vizsgálata
  uint8_t side_walls = infra[right] < sideInfraLimit && infra[rightdi] < diagonalInfraLimit && pastinfra[right] < sideInfraLimit && infra_deriv[right] < derivInfraLimit;
  //Bal fal vizsgálata
  side_walls += (infra[left] < sideInfraLimit && infra[leftdi] < diagonalInfraLimit && pastinfra[left] < sideInfraLimit && infra_deriv[left] < derivInfraLimit) << 1;
  return side_walls;
}


