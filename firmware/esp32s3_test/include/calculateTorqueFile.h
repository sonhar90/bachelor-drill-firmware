#pragma once
#include <Arduino.h>

extern float torque_est;
extern float torque_measured;

extern float grams_f;
extern float force_N;

extern float omega_chuck;
extern float power_mech;


void calculateTorqueMeasured(float grams);
