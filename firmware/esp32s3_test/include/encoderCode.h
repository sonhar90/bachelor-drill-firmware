#pragma once

#include <Arduino.h>

// ----- Public state -----
extern long encCount;
extern long encCountChange;

extern float radPerSec;
extern float degPerSec;
extern float rpmMotor;
extern float rpmChuck;

extern const float dt;

// ----- Setup / update -----
void encoderSetup();
void runEncoder();