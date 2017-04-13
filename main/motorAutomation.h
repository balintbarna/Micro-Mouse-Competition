//PID controllers
#define PTagSpeed 10
#define ITagSpeed 2
#define PTagCas 2

#define maxSpeed 500

//constants for recursive filter
#define wholePart 100000
#define filterDuty 0.1
const int newPart = wholePart * filterDuty;
const int oldPart = wholePart - newPart;
const int speedMultiplier = 1000000 / myinterval;

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
void SetMotorSpeed(int setSpeedLeft, int setSpeedRight)
{
  //Encoder érték kiolvasás
  leftPos = encoderLeft.read();
  rightPos = encoderRight.read();

  //Sebesség számítása + rekurzív szűrés sebességre
  aggrSpeedLeft = (oldPart * aggrSpeedLeft + newPart * speedMultiplier * (leftPos - leftPosOld)) / wholePart;
  aggrSpeedRight = (oldPart * aggrSpeedRight + newPart * speedMultiplier * (rightPos - rightPosOld)) / wholePart;

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

void CascadePos(int setPosLeft, int setPosRight)
{
  leftPos = encoderLeft.read();
  rightPos = encoderRight.read();

  int errorLeft = setPosLeft - leftPos;
  int errorRight = setPosRight - rightPos;

  //Getting outputs
  int opLeft = PTagCas * errorLeft;
  int opRight = PTagCas * errorRight;

  //Comparing and setting the values to maxSpeed
  if (abs(opLeft) > maxSpeed)
  {
    int ratio = maxSpeed * 2000 / abs(opLeft);
    opLeft *= ratio;
    opLeft /= 2000;
    opRight *= ratio;
    opRight /= 2000;
  }
  if (abs(opRight) > maxSpeed)
  {
    int ratio = maxSpeed * 2000 / abs(opRight);
    opLeft *= ratio;
    opLeft /= 2000;
    opRight *= ratio;
    opRight /= 2000;
  }

  SetMotorSpeed(opLeft, opRight);
}
