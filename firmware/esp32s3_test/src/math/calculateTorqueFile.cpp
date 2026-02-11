#include <Arduino.h>
#include "encoderCode.h"
#include "currentSens.h"
#include "calculateTorqueFile.h"

float powerOut = 0.0;
float radPerSecOut = 0.0;
float degPerSecOut = 0.0;
float rpm = 0.0;
float torqueOut = 0.0;

const float virkningsgrad_motor = 0.55;
const float gearConstant = 1.0/231.0;

void calculateTorque(){
  powerOut = power_mW/1000.0*virkningsgrad_motor;
  radPerSecOut = radPerSec*gearConstant;
  degPerSecOut = degPerSec*gearConstant;
  rpm = degPerSecOut*60.0/360.0;

  if (!(-0.001 < radPerSecOut && radPerSecOut < 0.001)){
    torqueOut = powerOut/radPerSecOut;
  } else {
    torqueOut = 0.0;
  }
}
