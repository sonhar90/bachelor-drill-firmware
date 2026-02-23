#include <Arduino.h>
#include <Wire.h>
#include "DFRobot_INA219.h"
#include "currentSens.h"

DFRobot_INA219_IIC ina219(&Wire, INA219_I2C_ADDRESS4);

float ina219Reading_mA = 1000;
float extMeterReading_mA = 1000;

float current_mA = 0.0;
float voltage_V = 0.0;
float power_mW  = 0.0;

// smoothing-state
static float currentFiltered = 0.0f;
static float voltageFiltered = 0.0f;

void currentSetup()
{
  Wire.begin(21, 22);
  Wire.setClock(100000);

  while (ina219.begin() != true)
  {
    if (Serial)
      Serial.println("INA219 begin failed");

    delay(2000);
  }

  ina219.linearCalibrate(ina219Reading_mA, extMeterReading_mA);
}

void currentRead()
{
  float newCurrent = ina219.getCurrent_mA();
  float newVoltage = ina219.getBusVoltage_V();

  // smoothing (fjerner jitter)
  currentFiltered = 0.8f * currentFiltered + 0.2f * newCurrent;
  voltageFiltered = 0.8f * voltageFiltered + 0.2f * newVoltage;

  current_mA = currentFiltered;
  voltage_V  = voltageFiltered;

  power_mW = current_mA * voltage_V;
}