//bal motor encoder bemenet
#define encoderLeftFront 12
#define encoderLeftBack 11
//jobb motor encoder bemenet
#define encoderRightFront 9
#define encoderRightBack 10
//bal motor encoder számláló
volatile double leftPosOld = 0;
//jobb motor encoder számláló
volatile double rightPosOld = 0;

#define ENCODER_OPTIMIZE_INTERRUPTS

//includes
#include <Encoder.h>

//init encoder objects
Encoder encoderLeft(encoderLeftFront, encoderLeftBack);
Encoder encoderRight(encoderRightFront, encoderRightBack);

//Resets encoder values
void ResetEncoders() {
  encoderLeft.write(0);
  encoderRight.write(0);
}




