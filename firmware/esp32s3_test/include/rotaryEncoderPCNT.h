#pragma once
#include <Arduino.h>

class RotaryEncoderPCNT {
public:
    RotaryEncoderPCNT(int pinA, int pinB);
    int32_t position();

private:
    void initPCNT(int pinA, int pinB);
};