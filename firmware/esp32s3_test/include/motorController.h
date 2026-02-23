#pragma once
#include <Arduino.h>

extern float motorPercentage;
extern int duty;
extern uint32_t lastMotorChangeTime;

void motorSetup();
void motorForward();
void motorBackward();
void motorStop();
void controlMotor();
void controlPercentage();
