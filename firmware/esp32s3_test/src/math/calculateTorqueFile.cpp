#include <Arduino.h>
#include <math.h>
#include "calculateTorqueFile.h"
#include "encoderCode.h"
#include "currentSens.h"

float torque_est = 0.0f;
float torque_measured = 0.0f;

float grams_f = 0.0f;
float force_N = 0.0f;

float omega_chuck = 0.0f;
float power_mech = 0.0f;

static constexpr float G = 9.81f;
static constexpr float ARM_LENGTH = 0.05f;

static constexpr float KT = 0.010f;
static constexpr float GEAR_RATIO = 231.0f;


void calculateTorqueMeasured(float grams)
{
  // deadband for lastcelle
  if (fabs(grams) < 10.0f) {   // < 5 gram = støy
    grams = 0.0f;
  }

  grams_f = grams;

  force_N = (grams / 1000.0f) * G;
  torque_measured = force_N * ARM_LENGTH;

  if (fabsf(torque_measured) < 0.0005f) {
    torque_measured = 0.0f;
  }

  omega_chuck = rpmChuck * 2.0f * PI / 60.0f;

  power_mech = torque_measured * omega_chuck;

  // ---- torque_est fra motorstrøm ----

// current fra INA219 (allerede global i prosjektet)
float current_A = current_mA / 1000.0f;

// rpm-baserte tomgangstap
float I0 = 0.00008f * rpm + 0.25f;

// effektiv strøm som gir moment
float I_eff = current_A - I0;
if (I_eff < 0.0f) I_eff = 0.0f;

// estimer moment på chuck
torque_est = KT * I_eff * GEAR_RATIO;

}
