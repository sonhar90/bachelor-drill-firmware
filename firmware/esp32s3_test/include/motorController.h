#pragma once
#include <Arduino.h>

extern float motorPercentage;
extern int duty;

void motorSetup();
void motorForward();
void motorBackward();
void motorStop();
void controlMotor();
void controlPercentage();
