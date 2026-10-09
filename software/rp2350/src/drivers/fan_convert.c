/**
 * @file fan_convert.c
 * @brief Pure fan PWM math. No hardware access, builds on the host.
 */
#include <stdint.h>

#include "drivers/fan.h"

#define PERCENT_MAX     100U

uint16_t fan_pwm_top(uint32_t clk_hz, uint32_t freq_hz)
{
    uint32_t ticks;

    if (freq_hz == 0U) {
        return (uint16_t)FAN_PWM_TOP_MAX;
    }

    ticks = clk_hz / freq_hz;    /* ticks per PWM cycle = TOP + 1 */

    if (ticks == 0U) {
        return 0U;               /* freq above clk: fastest possible */
    }
    if ((ticks - 1U) > FAN_PWM_TOP_MAX) {
        return (uint16_t)FAN_PWM_TOP_MAX;
    }
    return (uint16_t)(ticks - 1U);
}

uint16_t fan_percent_to_level(uint8_t percent, uint16_t top)
{
    uint32_t pct = percent;

    if (pct > PERCENT_MAX) {
        pct = PERCENT_MAX;
    }

    /* Multiply before dividing, in 32 bits: 100 * 65535 still fits */
    return (uint16_t)((pct * ((uint32_t)top + 1U)) / PERCENT_MAX);
}
