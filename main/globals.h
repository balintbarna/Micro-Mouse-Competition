//Variables for maze solver
volatile bool needPlanning = false;
volatile bool planningDone = false;
//#include <QueueList.h>
//QueueList<char> path;
String path = "";

//map size (32 for competition)
#define mapsize 8

//Goal coordinates
#define originalGoalX 3
#define originalGoalY 2
volatile int8_t goalX = originalGoalX;
volatile int8_t goalY = originalGoalY;

//Coordinates and commands used for movement
volatile int8_t nextX = 0;
volatile int8_t nextY = 0;
String action = "";

//Variables for position in a 2D matrix
//0;0 a kiindulási pont
volatile int8_t posX = 0;
volatile int8_t posY = 0;
volatile int8_t savedPosX = 0;
volatile int8_t savedPosY = 0;
volatile int32_t lastPosEncAvg = 0;

//Is it in the mid area of a cell
volatile bool midzone = true;

//0 az előre, -3től 4-ig irányt mutat
/*    7  0  1
      6     2
      5  4  3 */
volatile uint8_t orientation = 0;

//Setting pwm output parameters
#define pwmRes 10
const uint16_t pwmMax = pow(2, pwmRes);
const uint16_t pwmMid = pwmMax / 2;

//infra sensor values and TOF
volatile int32_t infra[5];
volatile int32_t pastinfra[5];
volatile int32_t infra_deriv[5];
volatile int32_t TOFread = 0;

//states
volatile char state = 'S';

//time measure
elapsedMillis milli = 0;
elapsedMillis delayTimer = 0;
elapsedMicros micro = 0;
elapsedMicros measurer = 0;
uint32_t measuredTime = 0;

//Variable to read command from serial
String serialCommand = "";
//Paramteres
volatile int32_t param1 = 0, param2 = 0, param3 = 0, param4 = 0;

//Encoder signals / sec  ---  0.28mm/sec
//Theoretical maximum is 6000
const int32_t maxSpeed = 1500;

//----------- Movement constants -----------
#define rotationCoeff 1.4
const int16_t fullRotationSum = 1050;
const int16_t positiveFullRotation = fullRotationSum * rotationCoeff / (rotationCoeff + 1);
const int16_t negativeFullRotation = -(fullRotationSum / (rotationCoeff + 1));

const int16_t encoderToMicro = 280;
const int16_t infraToMicro = 10;
const int16_t encoderToInfra = encoderToMicro / infraToMicro;

const int16_t breakLengthInfra = maxSpeed * encoderToInfra / 100;

//----------- Infra constants ---------------
const int16_t sideInfraLimit = 3000;
const int16_t diagonalInfraLimit = 5000;
const int16_t derivInfraLimit = 4;
const int16_t frontInfraLimit = 1300;
const int16_t midInfraValue = 1750;

//cell value max
const uint16_t cellValueMax = 9999;

//timer frequency and interval (microsec)
#define timerFrequency 500
const uint32_t myinterval = 1000000 / timerFrequency;

//general pins
#define infraPin 22
#define batteryPin A14
#define led0 13
#define gombPin 21
#define led1 2
#define led2 23
