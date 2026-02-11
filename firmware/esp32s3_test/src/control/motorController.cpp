#include <Arduino.h>
#include "motorController.h"

const int motorChAPin = 5;
const int motorChBPin = 6;
const int motorPWMPin = 7;

const int PWM_CH = 0;          // ledc kanal 0-7 (C3 har flere)
const int PWM_FREQ = 50000;    // 50 kHz
const int PWM_RES_BITS = 8;    // 0..255

float motorPercentage = 0.0;
float percentageChange = 0.02;
bool rising = true;
int duty;

void motorSetup(){
  pinMode(motorChAPin, OUTPUT);
  pinMode(motorChBPin, OUTPUT);

  ledcSetup(PWM_CH, PWM_FREQ, PWM_RES_BITS);
  ledcAttachPin(motorPWMPin, PWM_CH);
}

void motorForward(){
  digitalWrite(motorChAPin, HIGH);
  digitalWrite(motorChBPin, LOW);
}

void motorBackward(){
  digitalWrite(motorChAPin, LOW);
  digitalWrite(motorChBPin, HIGH);
}

void motorStop(){
  digitalWrite(motorChAPin, LOW);
  digitalWrite(motorChBPin, LOW);
}

void controlMotor(){
  if (motorPercentage < 0.0){
    duty = int(-255*motorPercentage);
  }
  else if (motorPercentage > 0.0){
    duty = int(255*motorPercentage);
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
}

void controlPercentage(){
  if (rising){
    motorPercentage += percentageChange;
  } else {
    motorPercentage -= percentageChange;
  }

  if(motorPercentage >= 0.98 && rising){
    rising = false;
  }
  else if (motorPercentage <= -0.98 && !rising){
    rising = true;
  }
}
