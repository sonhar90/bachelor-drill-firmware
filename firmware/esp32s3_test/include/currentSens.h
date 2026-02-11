#pragma once
#include <Arduino.h>

extern float current_mA;
extern float voltage_V;
extern float power_mW;

void currentSetup();
void currentRead();

