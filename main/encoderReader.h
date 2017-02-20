//bal motor encoder bemenet
#define irptP0 2
//jobb motor encoder bemenet
#define irptP1 3
//bal motor encoder számláló
long encoder0 = 0;
long lastPos0 = 0;
//jobb motor encoder számláló
long encoder1 = 0;
long lastPos1 = 0;

//bal motor számláló függvény
void add0() {
  encoder0++;
}
//jobb motor számláló függvény
void add1() {
  encoder1++;
}

//Call this to setup pins for encoders
void SetupEncoders() {
  pinMode(irptP0, INPUT_PULLUP);
  pinMode(irptP1, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(irptP0), add0, RISING);
  attachInterrupt(digitalPinToInterrupt(irptP1), add1, RISING);
}

void ResetEncoders() {
  encoder0 = 0;
  lastPos0 = 0;
  encoder1 = 0;
  lastPos1 = 0;
}




