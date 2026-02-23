#pragma once
#include <Arduino.h>

// ===== systemkonstanter =====
//constexpr float KT_REAL_GUESS = 0.0105f;
constexpr float GEAR_RATIO = 231.0f;

// ===== sluttresultater =====
extern float torque_est;
extern float torque_measured;

extern float grams_f;
extern float force_N;

extern float omega_chuck;
extern float power_mech;

// ===== strøm og modell =====
extern float current_A;
extern float I0_model;
extern float I_eff;

// ===== identifikasjonsparametre =====
extern float KT_i;
extern float friction_i;
extern float eta_i;

void calculateTorqueMeasured(float grams);