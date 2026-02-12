#pragma once
#include <Arduino.h>

extern float powerOut;
extern float radPerSecOut;
extern float degPerSecOut;
extern float rpm;
extern float torqueOut;
extern float torque_est;
extern float torque_measured;

void calculateTorque(float grams);
