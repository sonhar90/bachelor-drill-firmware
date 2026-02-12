#pragma once
#include <Arduino.h>

extern float radPerSec;
extern float degPerSec;
extern float rpm;
extern float rpmChuck;




void encoderSetup();
void runEncoder();
