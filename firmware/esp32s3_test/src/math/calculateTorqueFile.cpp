#include <Arduino.h>
#include <math.h>
#include "calculateTorqueFile.h"
#include "encoderCode.h"
#include "currentSens.h"
#include "config.h"
#include "motorController.h"

// ======================================================
// OUTPUT
// ======================================================

float torque_est       = 0.0f;
float torque_measured  = 0.0f;

float grams_f = 0.0f;
float force_N = 0.0f;

float omega_chuck = 0.0f;
float power_mech  = 0.0f;

float current_A = 0.0f;
float I0_model  = 0.0f;
float I_eff     = 0.0f;

float KT_i       = 0.0f;
float friction_i = 0.0f;
float eta_i      = 0.0f;

// ======================================================
// FYSISKE PARAMETRE
// ======================================================

static constexpr float G = 9.81f;
static constexpr float ARM_LENGTH = 0.05f;

static constexpr float KT_REAL     = 0.013f;
static constexpr float ETA_TOTAL   = 0.82f;

// ======================================================
// I0-MODELL
// ======================================================

static constexpr float I0_A = 1.90f;
static constexpr float I0_B = -0.000065f;

// ======================================================
// HOVEDFUNKSJON
// ======================================================

void calculateTorqueMeasured(float grams)
{
    if (millis() - lastMotorChangeTime < 150)
    {
    torque_est = 0;
    return;
    }

    if (rpmMotor < RPM_TORQUE_MIN)
    {
        torque_est = 0;
        return;
    }

    if (!isfinite(rpmMotor)) rpmMotor = 0.0f;
    if (!isfinite(rpmChuck)) rpmChuck = 0.0f;

    // -------------------------
    // MÅLT MOMENT
    // -------------------------

    grams_f = grams;

    force_N = (grams / 1000.0f) * G;
    torque_measured = force_N * ARM_LENGTH;

    // -------------------------
    // STRØM
    // -------------------------

    current_A = max(0.0f, current_mA / 1000.0f);
    if (!isfinite(current_A))
{
    torque_est = 0;
    return;
}

    // -------------------------
    // I0 fra rpmMotor
    // -------------------------

    float rpm_abs = fabs(rpmMotor);

    I0_model = I0_A + I0_B * rpm_abs;

    // -------------------------
    // Effektiv momentstrøm
    // -------------------------

    I_eff = current_A - I0_model;
    if (I_eff < 0.0f)
        I_eff = 0.0f;
    if (I_eff > 30.0f) I_eff = 30.0f;

    // -------------------------
    // Motor-moment
    // -------------------------

    float torque_motor = KT_REAL * I_eff;

    // -------------------------
    // Moment på chuck
    // -------------------------

    torque_est =
        torque_motor *
        GEAR_RATIO *
        ETA_TOTAL;

    // -------------------------
    // Mekanisk effekt
    // -------------------------

    omega_chuck = rpmChuck * 2.0f * PI / 60.0f;
    power_mech  = torque_est * omega_chuck;

    // -------------------------
    // Diagnoseparametre
    // -------------------------

    //KT_i = (I_eff > 0.01f)
    //    ? torque_motor / I_eff
    //    : 0.0f;
//
 //   eta_i = (torque_motor > 0.001f)
   //     ? torque_measured / (torque_motor * GEAR_RATIO)
    //    : 0.0f;
//
  //  friction_i = torque_est - torque_measured;
}