#include "rotaryEncoderPCNT.h"
#include "driver/pcnt.h"

#define PCNT_UNIT_USED    PCNT_UNIT_0
#define PCNT_CHANNEL_USED PCNT_CHANNEL_0

// Intern state for overflow-håndtering
static int16_t  lastRawCount = 0;
static int32_t  totalCount   = 0;

RotaryEncoderPCNT::RotaryEncoderPCNT(int pinA, int pinB) {
    initPCNT(pinA, pinB);
}

void RotaryEncoderPCNT::initPCNT(int pinA, int pinB)
{
    pcnt_config_t pcnt_config = {};

    pcnt_config.pulse_gpio_num = pinA;
    pcnt_config.ctrl_gpio_num  = pinB;
    pcnt_config.unit           = PCNT_UNIT_USED;
    pcnt_config.channel        = PCNT_CHANNEL_USED;

    pcnt_config.pos_mode = PCNT_COUNT_INC;
    pcnt_config.neg_mode = PCNT_COUNT_DEC;

    pcnt_config.lctrl_mode = PCNT_MODE_REVERSE;
    pcnt_config.hctrl_mode = PCNT_MODE_KEEP;

    pcnt_config.counter_h_lim = 32767;
    pcnt_config.counter_l_lim = -32768;

    pcnt_unit_config(&pcnt_config);

    pcnt_counter_pause(PCNT_UNIT_USED);
    pcnt_counter_clear(PCNT_UNIT_USED);
    pcnt_counter_resume(PCNT_UNIT_USED);

    lastRawCount = 0;
    totalCount   = 0;
}

// Returnerer kontinuerlig 32-bit posisjon uten overflow-spikes
int32_t RotaryEncoderPCNT::position()
{
    int16_t rawCount = 0;
    pcnt_get_counter_value(PCNT_UNIT_USED, &rawCount);

    int32_t delta = rawCount - lastRawCount;

    // Overflow-korrigering (16-bit wrap)
    if (delta > 32767)  delta -= 65536;
    if (delta < -32768) delta += 65536;

    totalCount += delta;
    lastRawCount = rawCount;

    return totalCount;
}