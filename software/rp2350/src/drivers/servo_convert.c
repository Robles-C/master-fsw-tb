/**
 * @file servo_convert.c
 * @brief Pure servo PWM math. No hardware access, builds on the host.
 */
#include <stdint.h>

#include "drivers/servo.h"

#define CLKDIV_MIN      1U
#define CLKDIV_MAX      255U      /* integer part of the PWM divider is 8 bits */
#define TOP_MAX         65535U

uint8_t servo_clkdiv(uint32_t clk_hz)
{
    uint32_t div = clk_hz / SERVO_TICK_HZ;

    if (div < CLKDIV_MIN) {
        div = CLKDIV_MIN;
    } else if (div > CLKDIV_MAX) {
        div = CLKDIV_MAX;
    } else {
        /* in range */
    }
    return (uint8_t)div;
}

uint16_t servo_pwm_top(uint32_t freq_hz)
{
    uint32_t ticks;

    if (freq_hz == 0U) {
        return (uint16_t)TOP_MAX;
    }
    /* ticks per PWM cycle = TOP + 1 */
    ticks = SERVO_TICK_HZ / freq_hz;    

    if (ticks == 0U) {
        return 0U;
    }
    if ((ticks - 1U) > TOP_MAX) {
        return (uint16_t)TOP_MAX;
    }
    return (uint16_t)(ticks - 1U);
}

uint16_t servo_angle_to_pulse_us(uint16_t angle_deg, uint16_t min_us,
                                 uint16_t max_us, uint16_t range_deg)
{
    uint32_t angle = angle_deg;

    /* Bad ranges would divide by zero or go negative: hold at min */
    if ((range_deg == 0U) || (max_us <= min_us)) {
        return min_us;
    }
    if (angle > range_deg) {
        angle = range_deg;
    }

    /* Multiply before dividing, in 32 bits */
    return (uint16_t)(min_us +
                      ((angle * ((uint32_t)max_us - min_us)) / range_deg));
}
