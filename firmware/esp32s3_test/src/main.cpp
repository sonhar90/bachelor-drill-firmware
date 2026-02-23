#include <Arduino.h>
#include "encoderCode.h"
#include "currentSens.h"
#include "calculateTorqueFile.h"
#include "motorController.h"
#include "config.h"

// ================= SETTINGS =================

static constexpr uint32_t SAMPLE_PERIOD_MS = 50;
static constexpr float MOTOR_FIXED_DUTY   = 0.60f;
static constexpr float DUTY_STEP          = 0.05f;

// ================= OVERLOAD =================

static int  overloadCounter = 0;
static bool overloadLatched = false;

// ================= STATE =================

uint32_t lastSample = 0;

bool loggingActive = false;
String currentFileName = "";

// ================= SETUP =================

void setup() {
  Serial.begin(115200);
  delay(500);

  encoderSetup();
  currentSetup();
  motorSetup();

  motorPercentage = 0.0f;
  controlMotor();

  Serial.println("READY");
  Serial.println("file <name>");
  Serial.println("start  -> start logging (motor OFF)");
  Serial.println("w      -> motor ON (60%)");
  Serial.println("s      -> motor OFF");
  Serial.println("u      -> duty +5%");
  Serial.println("d      -> duty -5%");
  Serial.println("stop   -> stop logging");
}

// ================= SERIAL HANDLER =================

void handleSerial()
{
  if (!Serial.available()) return;

  String cmd = Serial.readStringUntil('\n');
  cmd.trim();

  if (cmd.startsWith("file "))
  {
    currentFileName = cmd.substring(5);
    Serial.print("FILE SET: ");
    Serial.println(currentFileName);
  }

  else if (cmd == "start")
  {
    if (currentFileName.length() == 0)
    {
      Serial.println("ERROR: set filename first");
      return;
    }

    loggingActive = true;

    Serial.println("CSV_BEGIN");
    Serial.println("t_ms,rpm_motor,rpm_chuck,current_mA,voltage_V,power_mW,torque_est");
  }

  else if (cmd == "w")
  {
    overloadLatched = false;        // tillat start igjen etter stopp
    motorPercentage = MOTOR_FIXED_DUTY;
    controlMotor();
  }

  else if (cmd == "s")
  {
    motorPercentage = 0.0f;
    controlMotor();
  }

  else if (cmd == "u")
  {
    overloadLatched = false;
    motorPercentage += DUTY_STEP;
    if (motorPercentage > 0.95f) motorPercentage = 0.95f;
    controlMotor();

    Serial.print("DUTY: ");
    Serial.println(motorPercentage, 2);
  }

  else if (cmd == "d")
  {
    overloadLatched = false;
    motorPercentage -= DUTY_STEP;
    if (motorPercentage < 0.0f) motorPercentage = 0.0f;
    controlMotor();

    Serial.print("DUTY: ");
    Serial.println(motorPercentage, 2);
  }

  else if (cmd == "stop")
  {
    loggingActive = false;
    Serial.println("CSV_END");
  }
}

// ================= LOOP =================

void loop()
{
  handleSerial();

  uint32_t now = millis();

  if (now - lastSample >= SAMPLE_PERIOD_MS)
  {
    lastSample = now;

    runEncoder();
    currentRead();

    // torque estimator (uten loadcell under drilling)
    calculateTorqueMeasured(0.0f);

    // -------- OVERLOAD DETECTION --------

    if (torque_est > TORQUE_LIMIT)
        overloadCounter++;
    else
        overloadCounter = 0;

    if (overloadCounter >= OVERLOAD_LIMIT_SAMPLES)
    {
        motorStop();                // BRÅSTOPP
        overloadLatched = true;     // lås motor
        overloadCounter = 0;
    }

    // -------- LOGGING --------

    if (loggingActive)
    {
      Serial.print(now);
      Serial.print(",");
      Serial.print(rpmMotor, 2);
      Serial.print(",");
      Serial.print(rpmChuck, 2);
      Serial.print(",");
      Serial.print(current_mA, 1);
      Serial.print(",");
      Serial.print(voltage_V, 2);
      Serial.print(",");
      Serial.print(power_mW, 0);
      Serial.print(",");
      Serial.println(torque_est, 4);
    }

    // -------- NORMAL MOTOR CONTROL --------

    if (!overloadLatched)
    {
        controlMotor();
    }
  }
}