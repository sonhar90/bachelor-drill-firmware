#include <Arduino.h>
#include <Wire.h>
#include "DFRobot_INA219.h"
#include "currentSens.h"

/**
 * @fn DFRobot_INA219_IIC
 * @brief pWire I2C controller pointer
 * @param i2caddr  I2C address
 * @n INA219_I2C_ADDRESS1  0x40   A0 = 0  A1 = 0
 * @n INA219_I2C_ADDRESS2  0x41   A0 = 1  A1 = 0
 * @n INA219_I2C_ADDRESS3  0x44   A0 = 0  A1 = 1
 * @n INA219_I2C_ADDRESS4  0x45   A0 = 1  A1 = 1	 
  */
DFRobot_INA219_IIC     ina219(&Wire, INA219_I2C_ADDRESS4);

float ina219Reading_mA = 1000;
float extMeterReading_mA = 1000;

float current_mA = 0.0;
float voltage_V = 0.0;
float power_mW = 0.0;

void currentSetup(){
  Wire.begin(8, 9);        // SDA=8, SCL=9
  Wire.setClock(100000);   // valgfritt, men stabilt

  while(ina219.begin() != true) {
    if (Serial){
      Serial.println("INA219 begin failed");
    }
    delay(2000);
  }
  ina219.linearCalibrate(ina219Reading_mA, extMeterReading_mA);
}

void currentRead(){
  current_mA = ina219.getCurrent_mA();
  if (current_mA < 5){
    current_mA = 0.0;
  }
  voltage_V = ina219.getBusVoltage_V();
  power_mW = ina219.getPower_mW();
}
