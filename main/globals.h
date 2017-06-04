#include "Coord.h"
//timer frequency and interval (microsec)
#define timerFrequency 500
const uint32_t myinterval = 1000000 / timerFrequency;

//Variables for maze solver
volatile bool planningDone = false;
//#include <QueueList.h>
//QueueList<char> path;
String path = "";
char nextStep = 'S';

//map size (32 for competition)
#define mapsize 11

//Goal coordinates
#define originalGoalX 7
#define originalGoalY 4
Coord goal = {originalGoalX, originalGoalY};

//Coordinates and commands used for movement
String action = "";

//Variables for position in a 2D matrix
//0;0 a kiindulási pont
volatile Coord pos = {0, 0};
volatile Coord savedPos = {0, 0};
volatile int32_t lastPosEncAvg = 0;

//Is it in the mid area of a cell
volatile uint8_t cellMidZone = 1;
volatile uint8_t infraMidZone = 1;

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
//volatile int32_t TOFread = 0;

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

//----------- Movement constants -----------
//Encoder signals / sec  ---  0.28mm/sec
//Theoretical maximum is 6000
const int32_t maxSpeed = 500;

//PID controllers
const int32_t PTagSpeed = 400;
const int32_t ITagSpeed = 6 * myinterval / 1000;
const int32_t PTagCas = 15;

#define rotationCoeff 1.4
const int16_t fullRotationSum = 1100;
const int16_t positiveFullRotation = fullRotationSum * rotationCoeff / (rotationCoeff + 1);
const int16_t negativeFullRotation = -(fullRotationSum / (rotationCoeff + 1));

const int16_t encoderToMicro = 280;
const int16_t infraToMicro = 10;
const int16_t encoderToInfra = encoderToMicro / infraToMicro;

const int16_t breakLengthInfra = maxSpeed * encoderToInfra / 500;

//----------- Infra constants ---------------
//Parameters for infra based speed control
#define PInfraCoeff 1
#define DInfraCoeff 1
const int32_t PInfra = 1000 * PInfraCoeff;
const int32_t DInfra = 1000 * DInfraCoeff;

const int16_t sideInfraLimit = 3000;
const int16_t diagonalInfraLimit = 5500;
const int16_t derivInfraLimit = 4;
const int16_t frontInfraLimit = 1000;
const int16_t midInfraValue = 1700;

//cell value max
const uint16_t cellValueMax = 999;

//general pins
#define infraPin 22
#define batteryPin A14
#define led0 13
#define gombPin 21
#define led1 2
#define led2 23

//-------------- DEBUG ------------
volatile bool leftwall_debug = false;
volatile bool rightwall_debug = false;
