#pragma once
#include <Arduino.h>

class LoadCellHX711 {
public:
  void begin(int pinDout, int pinSck);
  void tare(uint8_t times = 15);

  long readRaw();
  long readTared(uint8_t samples = 10);

  float readGrams(float countsPerGram, uint8_t samples = 10);
  float calibrateCountsPerGram(float knownWeightGrams, uint8_t samples = 30);

  // === NYE (realtime-safe) ===
  bool isReady();
  float readGramsNonBlocking(float countsPerGram);

private:
  long offset_ = 0;
};
