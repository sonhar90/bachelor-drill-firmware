#include <Arduino.h>
#include "encoderCode.h"
#include "motorController.h"

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
  static uint8_t lastState = 0;

  uint8_t a = digitalRead(ENC_A);
  uint8_t b = digitalRead(ENC_B);
  uint8_t state = (a << 1) | b;

  // quadrature state machine
  if ((lastState == 0b00 && state == 0b01) ||
      (lastState == 0b01 && state == 0b11) ||
      (lastState == 0b11 && state == 0b10) ||
      (lastState == 0b10 && state == 0b00)) {
    encCount++;
  }
  else if ((lastState == 0b00 && state == 0b10) ||
           (lastState == 0b10 && state == 0b11) ||
           (lastState == 0b11 && state == 0b01) ||
           (lastState == 0b01 && state == 0b00)) {
    encCount--;
  }

  lastState = state;
}


void encoderSetup(){
  pinMode(ENC_A, INPUT_PULLUP);
  pinMode(ENC_B, INPUT_PULLUP);

  attachInterrupt(digitalPinToInterrupt(ENC_A), encISR, CHANGE);
  attachInterrupt(digitalPinToInterrupt(ENC_B), encISR, CHANGE);

  lastMillis = millis();
}

void runEncoder(){
  t = millis();

  if (t - lastMillis >= 100) {

    dt = (t - lastMillis) / 1000.0;

    encCountChange = encCount - lastEncCount;

    radPerSec = encCountChange * radPerCount / dt;
    degPerSec = encCountChange * degPerCount / dt;

    rpm = radPerSec * 60.0 / (2.0 * 3.14159);
    rpm = -rpm;
    rpmChuck = rpm / GEAR_RATIO;

    // ---- LEGG INN HER ----
    // stopp-modus: null rpm direkte
    if (motorPercentage == 0.0f) {
      rpm = 0.0f;
      rpmChuck = 0.0f;
    }
    if (fabs(rpm) < 5.0f) {
      rpm = 0.0f;
    }

    if (fabs(rpmChuck) < 0.1f) {
      rpmChuck = 0.0f;
    }
    // ----------------------

    lastEncCount = encCount;
    lastMillis = t;
  }

}

