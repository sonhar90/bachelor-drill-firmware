#include <Arduino.h>
#include "config.h"
#include "loadcell_hx711.h"

LoadCellHX711 loadcell;
float countsPerGram = HX711_COUNTS_PER_GRAM;

static void printHelp() {
  Serial.println("\n--- LOADCELL CAL MODE ---");
  Serial.println("t = tare (no load)");
  Serial.println("c = calibrate (place known weight, then press c)");
  Serial.println("p = print countsPerGram");
  Serial.println("r = print raw + tared (debug)");
}

void setup() {
  Serial.begin(115200);
  delay(500);

  loadcell.begin(PIN_HX711_DOUT, PIN_HX711_SCK);
  loadcell.tare();

  Serial.println("Loadcell ready.");
  printHelp();
}

void loop() {
  // Commands
  while (Serial.available()) {
    char ch = (char)Serial.read();

    if (ch == 't') {
      Serial.println("Tare: remove load...");
      loadcell.tare();
      Serial.println("Tare done.");
    }

    else if (ch == 'c') {
      Serial.print("Calibrating using ");
      Serial.print(CAL_WEIGHT_GRAMS, 1);
      Serial.println(" g (weight must be ON the cell now)...");
      float newCpg = loadcell.calibrateCountsPerGram(CAL_WEIGHT_GRAMS, 50);
      Serial.print("RESULT countsPerGram = ");
      Serial.println(newCpg, 6);
      Serial.println("-> Put this into config.h as HX711_COUNTS_PER_GRAM");
      countsPerGram = newCpg;
    }

    else if (ch == 'p') {
      Serial.print("countsPerGram = ");
      Serial.println(countsPerGram, 6);
    }

    else if (ch == 'r') {
      long raw = loadcell.readRaw();
      long tared = loadcell.readTared(20);
      Serial.print("raw=");
      Serial.print(raw);
      Serial.print(" tared=");
      Serial.println(tared);
    }

    else if (ch == 'h' || ch == '?') {
      printHelp();
    }
  }

  // Live grams output (rolig og nyttig under kalibrering)
  float grams = loadcell.readGrams(countsPerGram, 20);
  Serial.print("grams=");
  Serial.println(grams, 1);

  delay(200);
}
