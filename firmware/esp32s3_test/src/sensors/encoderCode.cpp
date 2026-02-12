#include <Arduino.h>
#include "encoderCode.h"

// -------- Encoder configuration --------
#define ENC_A 3
#define ENC_B 4

long encCount = 0;
long encCountChange = 0;
long lastEncCount = 0;

const float GEAR_RATIO = 231.0;


float radPerSec = 0.0;
float degPerSec = 0.0;
float rpm = 0.0;          // motor rpm
float rpmChuck = 0.0;     // chuck rpm


const float PPR = 1024.0;
const float CPR = PPR*1;
const float degPerCount = 360.0 / CPR;
const float radPerCount = 2*3.14159/CPR;

float dt = 0.2;
float t = 0;
long lastMillis = 0;

void IRAM_ATTR encISR() {
  if (digitalRead(ENC_B)) {
    encCount--;
  } else {
    encCount++;
  }
}

void encoderSetup(){
  pinMode(ENC_A, INPUT_PULLUP);
  pinMode(ENC_B, INPUT_PULLUP);

  attachInterrupt(digitalPinToInterrupt(ENC_A), encISR, RISING);
  lastMillis = millis();
}

void runEncoder(){
  t = millis();

  // oppdater rpm hvert 100 ms i stedet for hver loop
  if (t - lastMillis >= 100) {

    dt = (t - lastMillis) / 1000.0;

    encCountChange = encCount - lastEncCount;

    radPerSec = encCountChange * radPerCount / dt;
    degPerSec = encCountChange * degPerCount / dt;

    rpm = radPerSec * 60.0 / (2.0 * 3.14159);
    rpmChuck = rpm / GEAR_RATIO;


    lastEncCount = encCount;
    lastMillis = t;
  }
}

