//bal motor encoder bemenet
#define encoderLeftFront 12
#define encoderLeftBack 11
//jobb motor encoder bemenet
#define encoderRightFront 9
#define encoderRightBack 10

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




