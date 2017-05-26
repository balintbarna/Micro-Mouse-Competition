#define left 0
#define leftdi 1
#define front 2
#define rightdi 3
#define right 4
//Meredekség tömb
const int32_t m[5][16] = {
  {55555,1976,1785,3424,6756,10416,15625,21739,31250,38461,45454,62500,71428,100000,125000,166666},
  {10416,1811,2525,3759,5555,10000,13888,20833,27777,35714,50000,62500,83333,100000,166666,166666},
  {11111,1319,2538,4854,9259,10869,19230,23809,35714,45454,50000,83333,83333,100000,125000,125000},
  {2551,1886,3048,5617,5813,15151,25000,31250,35714,55555,62500,55555,166666,100000,125000,166666},
  {35714,1449,2066,4132,7575,11904,15625,25000,41666,45454,55555,71428,100000,125000,125000,125000}
};

//Y offset tömb
const int32_t b[5][16] = {
  {-3166666,369565,430357,-551369,-3033783,-6031250,-10546875,-16043478,-24812500,-31576923,-38227272,-54625000,-63285714,-91200000,-115750000,-156833333},
  {-625000,304347,30303,-687969,-1972222,-5550000,-8875000,-15062500,-21416666,-28821428,-42350000,-54312500,-74416666,-90600000,-155666666,-155666666},
  { -677777,360158,-230964,-1810679,-5268518,-6619565,-14019230,-18190476,-29285714,-38500000,-42850000,-75083333,-75083333,-91400000,-116000000,-116000000},
  { -173469,1886,-612804,-2393258,-2546511,-10651515,-19525000,-25281250,-29464285,-48333333,-55000000,-48277777,-156833333,-91500000,-116125000,-157333333},
  { -2071428,395652,138429,-1223140,-3909090,-7571428,-10875000,-19500000,-35166666,-38772727,-48500000,-63928571,-91900000,-116500000,-116500000,-116500000}
};

//Threshold tömb
const int32_t thr[5][17] = {
  {55, 63, 377, 596, 720, 798, 848, 884, 909, 928, 940, 953, 961, 966, 971, 977, 980},
  {58, 115, 427, 620, 716, 795, 836, 871, 899, 915, 932, 945, 953, 959, 963, 968, 973},
  {58, 68, 303, 567, 708, 795, 856, 894, 918, 936, 949, 960, 968, 975, 980, 985, 988},
  {65, 205, 498, 658, 762, 840, 879, 906, 921, 939, 949, 958, 965, 970, 973, 977, 980},
  {59, 65, 365, 600, 729, 824, 871, 909, 928, 946, 957, 968, 974, 980, 985, 989, 992}
};


//Analog input pins
const int32_t inputs[5] = {A3, A6, A2, A1, A0};

//Read infra values, index: 0:left, 1:left-diagonal, 2:front, 3:right-diagonal, 4:right
void _readInfraPin(int8_t index)
{
  pastinfra[index] = infra[index];

  int16_t value = analogRead(inputs[index]);
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

uint8_t _infraCounter = 0;
void InfraISR()
{
  if (_infraCounter == 1)
  {
    digitalWrite(infraPin, 1);
  }
  if (_infraCounter == 2)
  {
    _readInfra();
    digitalWrite(infraPin, 0);
    micro = 0;
  }
  if (_infraCounter == 4)
  {
    _infraCounter = 0;
  }
  _infraCounter++;
}

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
  int temp = TOF.readRangeSingleMillimeters();
  if (TOF.timeoutOccurred())
  {
    temp = 999999;
  }
  return temp;
}

