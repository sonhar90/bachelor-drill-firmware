#include <Arduino.h>
#include "motorController.h"

const int motorChAPin = 32;
const int motorChBPin = 25;
const int motorPWMPin = 33;

const int PWM_CH = 0;
const int PWM_FREQ = 5000;
const int PWM_RES_BITS = 8;

float motorPercentage = 0.0;
float percentageChange = 0.02;
bool rising = true;
int duty = 0;
uint32_t lastMotorChangeTime = 0;

void motorSetup(){
  pinMode(motorChAPin, OUTPUT);
  pinMode(motorChBPin, OUTPUT);

  ledcSetup(PWM_CH, PWM_FREQ, PWM_RES_BITS);
  ledcAttachPin(motorPWMPin, PWM_CH);
}

void motorForward(){
  digitalWrite(motorChAPin, HIGH);
  digitalWrite(motorChBPin, LOW);
  lastMotorChangeTime = millis();
}

void motorBackward(){
  digitalWrite(motorChAPin, LOW);
  digitalWrite(motorChBPin, HIGH);
  lastMotorChangeTime = millis();
}

void motorStop(){
  digitalWrite(motorChAPin, LOW);
  digitalWrite(motorChBPin, LOW);
  lastMotorChangeTime = millis();
}

void controlMotor(){
  if (motorPercentage < 0.0){
    duty = int(-255*motorPercentage);
  }
  else if (motorPercentage > 0.0){
    duty = int(255*motorPercentage);
  }
  else{
    duty = 0;
  }

  duty = constrain(duty, 0, 255);

  if (motorPercentage > 0.0){
    motorForward();
    ledcWrite(PWM_CH, duty);
  }
  else if (motorPercentage < 0.0){
    motorBackward();
    ledcWrite(PWM_CH, duty);
  }
  else{
    motorStop();
    ledcWrite(PWM_CH, 0);
  }
  lastMotorChangeTime = millis();
}

void controlPercentage(){
  if (rising){
    motorPercentage += percentageChange;
  } else {
    motorPercentage -= percentageChange;
  }

  if(motorPercentage >= 0.80 && rising){
    rising = false;
  }
  else if (motorPercentage <= 0.20 && !rising){
    rising = true;
  }
}
