#include <Arduino.h>

#include "encoderCode.h"
#include "currentSens.h"
#include "calculateTorqueFile.h"
#include "motorController.h"
#include "loadcell_hx711.h"
#include "config.h"

long loopTimer = 0;

LoadCellHX711 loadcell;
float countsPerGram = HX711_COUNTS_PER_GRAM;

void setup() {
  Serial.begin(115200);
  delay(500);

  encoderSetup();
  currentSetup();
  motorSetup();

  loadcell.begin(PIN_HX711_DOUT, PIN_HX711_SCK);
  loadcell.tare();

  // CSV HEADER – MÅ være første linje
  Serial.println("time_ms,rpm_motor,rpm_chuck,current_A,voltage_V,grams,torque_measured_Nm,torque_est_Nm");


  // Litt delay før tekst
  delay(500);

  Serial.println("PRONY TORQUE TEST MODE");
  Serial.println("1 = low speed");
  Serial.println("2 = medium");
  Serial.println("3 = high");
  Serial.println("s = stop");
}

void loop(){

  // tastestyring
  while (Serial.available()) {
    char ch = Serial.read();

    if (ch == '1') motorPercentage = 0.25;
    if (ch == '2') motorPercentage = 0.45;
    if (ch == '3') motorPercentage = 0.70;
    if (ch == 's') motorPercentage = 0.0;
  }

  if (millis() > (loopTimer + 50)){

    runEncoder();
    currentRead();

    float grams = loadcell.readGrams(countsPerGram, 10);

    calculateTorque(grams);

    // CSV DATA
    Serial.print(millis());
    Serial.print(",");

    Serial.print(rpm);
    Serial.print(",");

    Serial.print(rpmChuck);
    Serial.print(",");

    Serial.print(current_mA/1000.0);
    Serial.print(",");

    Serial.print(voltage_V);
    Serial.print(",");

    Serial.print(grams);
    Serial.print(",");

    Serial.print(torque_measured);
    Serial.print(",");

    Serial.println(torque_est);


    controlMotor();
    loopTimer = millis();
  }
}
