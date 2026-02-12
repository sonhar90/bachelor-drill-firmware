#include <Arduino.h>
#include "encoderCode.h"
#include "currentSens.h"
#include "calculateTorqueFile.h"

float powerOut = 0.0;
float torque_est = 0.0;
float torque_measured = 0.0;

const float ETA_MOTOR = 0.55;
const float GEAR_RATIO = 231.0;
const float ARM_LENGTH = 0.05;   // 5 cm

const float P_IDLE = 8.9;  // watt, målt fra tomgang

void calculateTorque(float grams){

  // ----- MÅLT TORQUE FRA LASTCELLE -----
  float force_N = (grams / 1000.0) * 9.81;
  torque_measured = force_N * ARM_LENGTH;

  // ----- ESTIMERT TORQUE FRA STRØM + RPM -----

  // elektrisk effekt inn i motor
  float power_elec = voltage_V * (current_mA / 1000.0);

  // trekk fra tomgangstap
  float power_mech_motor = (power_elec - P_IDLE) * ETA_MOTOR;

  // under tomgang → ingen torque-estimat
  if (power_mech_motor <= 0.0) {
    torque_est = 0.0;
    return;
  }

  float omega_motor = radPerSec;

  // lav hastighet → modellen er ikke gyldig
  if (fabs(omega_motor) < 30.0) {
    torque_est = 0.0;
    return;
  }

  // -------- TRANSIENT-FILTER --------
  // ignorer torque når rpm endrer seg raskt (hastighetsbytte / oppstart)
  static float rpm_prev = 0.0;
  float rpm_change = fabs(rpm - rpm_prev);
  rpm_prev = rpm;

  if (rpm_change > 200.0) {
    torque_est = 0.0;
    return;
  }
  // ----------------------------------

  // moment på motorside
  float torque_motor_est = power_mech_motor / omega_motor;

  // moment på chuck etter gir
  torque_est = torque_motor_est * GEAR_RATIO;

  // spike-beskyttelse
  if (torque_est > 20.0) {
      torque_est = 20.0;
  }
}
