//map size (32 for competition)
#define mapsize 5

//general pins
#define infraPin 22
#define batteryPin A14
#define led0 13
#define gombPin 21
#define led1 2
#define led2 23

//timer frequency and interval (microsec)
#define timerFrequency 500
const uint32_t myinterval = 1000000 / timerFrequency;

//Setting pwm output parameters
#define pwmRes 10
const uint16_t pwmMax = pow(2, pwmRes);
const uint16_t pwmMid = pwmMax / 2;

//infra sensor values and TOF
volatile int32_t infra[5] = {0, 0, 0, 0, 0};
volatile int32_t pastinfra[5] = {0, 0, 0, 0, 0};
volatile int32_t infra_deriv[5] = {0, 0, 0, 0, 0};
volatile int32_t TOFread = 0;

//states
volatile char state = 'S';

//time measure
elapsedMillis milli = 0;
elapsedMillis delayTimer = 0;
elapsedMicros micro = 0;

//Variable to read command from serial
String serialCommand = "";
//Paramteres
volatile int32_t param1 = 0, param2 = 0, param3 = 0, param4 = 0;

//Movement constants
const int16_t positiveFullRotation = 580;
const int16_t negativeFullRotation = -460;

//Infra constants
const int16_t sideInfraLimit = 3000;
const int16_t diagonalInfraLimit = 5000;
const int16_t derivInfraLimit = 4;
const int16_t frontInfraLimit = 1500;

