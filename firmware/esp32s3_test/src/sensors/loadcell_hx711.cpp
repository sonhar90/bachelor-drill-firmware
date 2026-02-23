#include <Arduino.h>
#include <HX711.h>
#include "loadcell_hx711.h"

static HX711 scale;

void LoadCellHX711::begin(int pinDout, int pinSck) {
  scale.begin(pinDout, pinSck);
  delay(200);
}

void LoadCellHX711::tare(uint8_t times) {
  scale.tare(times);
  offset_ = scale.get_offset();
}

long LoadCellHX711::readRaw() {
  return scale.read();
}

long LoadCellHX711::readTared(uint8_t samples) {
  long sum = 0;
  for (uint8_t i = 0; i < samples; i++) {
    sum += scale.read();
  }
  long avg = sum / samples;
  return avg - offset_;
}

float LoadCellHX711::readGrams(float countsPerGram, uint8_t samples) {
  if (countsPerGram == 0.0f) return 0.0f;
  return (float)readTared(samples) / countsPerGram;
}

float LoadCellHX711::calibrateCountsPerGram(float knownWeightGrams, uint8_t samples) {
  long t = readTared(samples);
  if (knownWeightGrams == 0.0f) return 0.0f;
  return (float)t / knownWeightGrams;
}

// ===== REALTIME SAFE =====

bool LoadCellHX711::isReady() {
  return scale.is_ready();
}

float LoadCellHX711::readGramsNonBlocking(float countsPerGram) {
  if (!scale.is_ready())
    return NAN;

  long value = scale.read() - offset_;
  return (float)value / countsPerGram;
}
