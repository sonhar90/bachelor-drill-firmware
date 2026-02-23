#include "encoderCode.h"
#include "calculateTorqueFile.h"
#include <rotaryEncoderPCNT.h>

// ---------- PIN CONFIG ----------
#define ENC_A 26
#define ENC_B 27

// ---------- HARDWARE PCNT ----------
RotaryEncoderPCNT encoder(ENC_A, ENC_B);

// ---------- STATE ----------
long encCount = 0;
long encCountChange = 0;
long lastEncCount = 0;

const float dt = 0.05; 

float radPerSec = 0.0f;
float degPerSec = 0.0f;
float rpmMotor = 0.0f;
float rpmChuck = 0.0f;

// ---------- ENCODER CONSTANTS ----------
static constexpr float PPR = 1024.0f;
static constexpr float CPR = PPR * 4.0f;         // quadrature x4
static constexpr float degPerCount = 360.0f / CPR;
static constexpr float radPerCount = 2.0f * PI / CPR;

// ---------- GEAR ----------
//static constexpr float GEAR_RATIO = 231.0f;

// ---------- TIMING ----------
//static unsigned long lastMillis = 0;


// =================================================
// SETUP
// =================================================

void encoderSetup()
{
    
}


// =================================================
void runEncoder()
{
    // les hardware teller (allerede overflow-sikret i PCNT-klassen)
    encCount = encoder.position();

    // pulsendring
    encCountChange = encCount - lastEncCount;

    // ny hastighet (rå)
    float newRadPerSec = encCountChange * radPerCount / dt;
    float newDegPerSec = encCountChange * degPerCount / dt;
    float newRpmMotor  = newRadPerSec * 60.0f / (2.0f * PI);

    // smoothing (fjerner jitter)
    radPerSec = 0.8f * radPerSec + 0.2f * newRadPerSec;
    degPerSec = 0.8f * degPerSec + 0.2f * newDegPerSec;
    rpmMotor  = 0.8f * rpmMotor  + 0.2f * newRpmMotor;

    rpmChuck = rpmMotor / GEAR_RATIO;

    // oppdater state
    lastEncCount = encCount;
}