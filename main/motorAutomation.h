//PID controllers
const int32_t PTagSpeed = 400;
const int32_t ITagSpeed = 2 * myinterval / 1000;
#define PTagCas 10

const int32_t maxSpeed = 325;
const int32_t midDistance = 2010;

//Parameters for infra based speed control
#define PInfraCoeff 0.5
#define DInfraCoeff 0.3
const int32_t PInfra = 1000 * PInfraCoeff;
const int32_t DInfra = 1000 * DInfraCoeff;

//constants for recursive filter
#define wholePart 100000
#define filterDutyDefault 0.1
#define filterOffRatio 2
const int minNewPart = wholePart * filterDutyDefault;
volatile int newPart = minNewPart;
volatile int oldPart = wholePart - newPart;
const int filterLowSpeed = filterOffRatio / 10.0 * timerFrequency;
const int filterHighSpeed = filterOffRatio * timerFrequency;

//Tárolók
//Positions
volatile int leftPos = 0;
volatile int rightPos = 0;
//szűrt sebesség hiba
volatile int aggrSpeedLeft = 0;
volatile int aggrSpeedRight = 0;
//integráló tag
volatile long long errSumLeft = 0;
volatile long long errSumRight = 0;
//régi pozicio
volatile int leftPosOld = 0;
volatile int rightPosOld = 0;

//Speed control PI
void SetMotorSpeed(int setSpeedLeft, int setSpeedRight, bool doWall = 0)
{
  //Normalizing the values to maxSpeed
  if (abs(setSpeedLeft) > maxSpeed)
  {
    int ratio = maxSpeed * 2000 / abs(setSpeedLeft);
    setSpeedLeft *= ratio;
    setSpeedLeft /= 2000;
    setSpeedRight *= ratio;
    setSpeedRight /= 2000;
  }
  if (abs(setSpeedRight) > maxSpeed)
  {
    int ratio = maxSpeed * 2000 / abs(setSpeedRight);
    setSpeedLeft *= ratio;
    setSpeedLeft /= 2000;
    setSpeedRight *= ratio;
    setSpeedRight /= 2000;
  }

  jobboldali = false;
  baloldali = false;
  //If we want to control wall proximity
  if (doWall)
  {
    int de = 0;
    int de_deriv = 0;
    /* wall_fitness
       1: jobb fal jó
       2: bal fal jó
       3: mindkét fal jó
    */
    //Jobb fal vizsgálata
    byte wall_fitness = infra[right] < 3500 && infra[rightdi] < 5000 && pastinfra[right] < 4500 && infra_deriv[right] < 4;
    //Bal fal vizsgálata
    wall_fitness += (infra[left] < 3500 && infra[leftdi] < 5000 && pastinfra[left] < 4500 && infra_deriv[left] < 4) << 1;
    //Ha van jó fal
    if (wall_fitness)
    {
      //Ha mindkét fal jó
      if (wall_fitness == 3)
      {
        //Ha a jobb fal közelebb van
        if (infra[right] < infra[left])
        {
          jobboldali = true;
          de = midDistance - infra[right];
          de_deriv = infra_deriv[right];
        }
        //Ha messzebb
        else
        {
          baloldali = true;
          de = infra[left] - midDistance;
          de_deriv = -infra_deriv[left];
        }
      }
      //Ha csak a jobb
      else if (wall_fitness == 1)
      {
        jobboldali = true;
        de = midDistance - infra[right];
        de_deriv = infra_deriv[right];
      }
      //Ha csak a bal
      else if (wall_fitness == 2)
      {
        baloldali = true;
        de = infra[left] - midDistance;
        de_deriv = -infra_deriv[left];
      }
    }
    setSpeedLeft -= (de * PInfra - de_deriv * DInfra) / 1000;
    setSpeedRight += (de * PInfra - de_deriv * DInfra) / 1000;
  }
  //Encoder érték kiolvasás
  leftPos = encoderLeft.read();
  rightPos = encoderRight.read();

  //Sebesség számítása + rekurzív szűrés sebességre
  aggrSpeedLeft = (oldPart * aggrSpeedLeft + newPart * timerFrequency * (leftPos - leftPosOld)) / wholePart;
  aggrSpeedRight = (oldPart * aggrSpeedRight + newPart * timerFrequency * (rightPos - rightPosOld)) / wholePart;

  //Setting filter
  int lesserSpeed = abs(aggrSpeedLeft) < abs(aggrSpeedRight) ? abs(aggrSpeedLeft) : abs(aggrSpeedRight);
  if (lesserSpeed > filterHighSpeed)
    newPart = wholePart;
  else if (lesserSpeed < filterLowSpeed)
    newPart = minNewPart;
  else
    newPart = map(lesserSpeed, filterLowSpeed, filterHighSpeed, minNewPart, wholePart);
  oldPart = wholePart - newPart;

  //Sebesség hiba számítása
  int errorLeft = setSpeedLeft - aggrSpeedLeft;
  int errorRight = setSpeedRight - aggrSpeedRight;

  //Tárolóba pozíció mentés
  leftPosOld = leftPos;
  rightPosOld = rightPos;

  //Integráló tag növelése
  errSumLeft += errorLeft;
  errSumRight += errorRight;

  //PI control
  int opLeft = PTagSpeed * errorLeft + ITagSpeed * errSumLeft;
  int opRight = PTagSpeed * errorRight + ITagSpeed * errSumRight;

  //Send output
  SetMotorPower(opLeft, opRight);
}

void CascadePos(int setPosLeft, int setPosRight, bool doWall = 0)
{
  leftPos = encoderLeft.read();
  rightPos = encoderRight.read();

  int errorLeft = setPosLeft - leftPos;
  int errorRight = setPosRight - rightPos;

  //Getting outputs
  int opLeft = PTagCas * errorLeft;
  int opRight = PTagCas * errorRight;

  SetMotorSpeed(opLeft, opRight, doWall);
}
