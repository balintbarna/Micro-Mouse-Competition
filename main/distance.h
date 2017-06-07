#define left 0
#define leftdi 1
#define front 2
#define rightdi 3
#define right 4
//Meredekség tömb
const int32_t m[5][16] = {
  {62500, 1661, 2024, 4166, 5747, 12820, 15625, 22727, 31250, 38461, 55555, 62500, 71428, 100000, 125000, 166666},
  {1742, 2427, 4950, 5813, 7142, 13157, 19230, 27777, 31250, 35714, 55555, 83333, 100000, 100000, 166666, 166666},
  {38461, 1597, 2252, 3906, 7142, 9433, 15625, 21739, 26315, 31250, 55555, 55555, 83333, 100000, 125000, 125000},
  {2164, 2032, 3289, 5434, 9259, 13513, 21739, 38461, 45454, 38461, 27777, 83333, 83333, 100000, 125000, 125000},
  {71428, 1547, 2272, 3472, 6410, 10416, 16666, 25000, 29411, 50000, 62500, 62500, 125000, 500000, 250000, 500000}
};

//Y offset tömb
const int32_t b[5][16] = {
  { -425000, 236842, -1100000, -2275862, -8153846, -10593750, -17000000, -24875000, -31653846, -47944444, -54625000, -63285714, -91200000, -115750000, -156833333},
  { -120209, -364077, -1782178, -2354651, -3350000 - 8276315, -13480769, -21027777, -24156250, -28250000, -46722222, -72833333, -88600000, -88600000, -152333333, -152333333},
  { -2192307, 388178, 137387, -863281, -3235714, -5075471, -10375000, -15804347, -19973684, -24562500, -47555555, -47555555, -74333333, -90500000, -114875000, -114875000},
  { -140692, -101626, -782894, -2271739, -5277777, -8851351, -16065217, -31115384, -37500000, -31038461, -21027777, -74083333, -74083333, -90200000, -114500000, -114500000},
  { -4142857, 399380, 118181, -611111, -2820512, -6145833, -11633333, -19200000, -23294117, -42750000, -54687500, -54687500, -115375000, -481000000, -237000000, -481500000}
};

//Threshold tömb
const int32_t thr[5][17] = {
  {68, 76, 377, 624, 744, 831, 870, 902, 924, 940, 953, 962, 970, 977, 982, 986, 989},
  {69, 356, 562, 663, 749, 819, 857, 883, 901, 917, 931, 940, 946, 951, 956, 959, 962},
  {57, 70, 383, 605, 733, 803, 856, 888, 911, 930, 946, 955, 964, 970, 975, 979, 983},
  {65, 296, 542, 694, 786, 840, 877, 900, 913, 924, 937, 955, 961, 967, 972, 976, 980},
  {58, 65, 388, 608, 752, 830, 878, 908, 928, 945, 955, 963, 971, 975, 976, 978, 979}
};


//Analog input pins
const int32_t inputs[5] = {A3, A6, A2, A1, A0};
volatile int32_t _calib = 0;
void _calibrateInfra()
{
    digitalWrite(infraPin, HIGH);
    delay(1);
    _calib = (1400 - (analogRead(inputs[left]) + analogRead(inputs[right]))) / 2;
    digitalWrite(infraPin, LOW);
}


//Read infra values, index: 0:left, 1:left-diagonal, 2:front, 3:right-diagonal, 4:right
void _readInfraPin(int8_t index)
{
  pastinfra[index] = infra[index];

  int32_t value = analogRead(inputs[index]) + _calib;
  if (value > thr[index][16])
  {
    infra[index] = 999999;
  }
  else if (value < thr[index][0])
  {
    infra[index] = 0;
  }
  else
  {
    for (int8_t i = 15; i >= 0; i--)
    {
      if (value > thr[index][i])
      {
        infra[index] = (value * m[index][i] + b[index][i]) / 1000;
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


