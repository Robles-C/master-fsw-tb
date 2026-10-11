/**
 * @file motor_convert.c
 * @brief Pure motor PWM math. No hardware access, builds on the host.
 */
#include <stdint.h>

#include "drivers/motor.h"

#define CMD_MAX     100

uint16_t motor_pwm_top(uint32_t clk_hz, uint32_t freq_hz)
{
    uint32_t ticks;

    if (freq_hz == 0U) {
        return (uint16_t)MOTOR_PWM_TOP_MAX;
    }

    ticks = clk_hz / freq_hz;    /* ticks per PWM cycle = TOP + 1 */

    if (ticks == 0U) {
        return 0U;               /* freq above clk: fastest possible */
    }
    if ((ticks - 1U) > MOTOR_PWM_TOP_MAX) {
        return (uint16_t)MOTOR_PWM_TOP_MAX;
    }
    return (uint16_t)(ticks - 1U);
}

motor_levels_t motor_cmd_to_levels(int8_t cmd, uint16_t top)
{
    /* Widen before clamping and negating: -(-128) doesn't fit in int8_t */
    int32_t c = cmd;
    const uint32_t full = (uint32_t)top + 1U;
    uint32_t drive;
    motor_levels_t lv = { 0U, 0U };     /* coast */

    if (c > CMD_MAX) {
        c = CMD_MAX;
    } else if (c < -CMD_MAX) {
        c = -CMD_MAX;
    } else {
        /* in range */
    }

    /* Multiply before dividing, in 32 bits: 100 * 65535 still fits */
    drive = ((uint32_t)((c < 0) ? -c : c) * full) / (uint32_t)CMD_MAX;

    if (c > 0) {
        lv.in1 = (uint16_t)full;
        lv.in2 = (uint16_t)(full - drive);  /* brakes the rest of the cycle */
    } else if (c < 0) {
        lv.in1 = (uint16_t)(full - drive);
        lv.in2 = (uint16_t)full;
    } else {
        /* 0: stay coasting */
    }
    return lv;
}
