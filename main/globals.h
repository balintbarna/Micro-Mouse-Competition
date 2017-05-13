//map size (32 for competition)
#define mapsize 5

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
