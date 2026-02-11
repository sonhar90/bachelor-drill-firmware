#pragma once
#include <Arduino.h>

extern float powerOut;
extern float radPerSecOut;
extern float degPerSecOut;
extern float rpm;
extern float torqueOut;

void calculateTorque();
