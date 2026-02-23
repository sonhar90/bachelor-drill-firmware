#pragma once

// HX711 pins (slik du har koblet)
constexpr int PIN_HX711_DOUT = 2;  // DT/DOUT
constexpr int PIN_HX711_SCK  = 4;  // SCK/CLK

// Kalibrering: fylles inn etter kalibrering
constexpr float HX711_COUNTS_PER_GRAM = 208.629410f;
constexpr float CAL_WEIGHT_GRAMS = 1020.0f;

// Overload Variabler

constexpr int   OVERLOAD_LIMIT_SAMPLES = 3;
constexpr float TORQUE_LIMIT = 0.45f;
constexpr int   RPM_TORQUE_MIN = 300;