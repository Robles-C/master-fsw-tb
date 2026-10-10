/**
 * @file fan_convert.c
 * @brief Pure fan PWM math. No hardware access, builds on the host.
 */
#include <stdint.h>

#include "drivers/fan.h"

#define PERCENT_MAX     100U
#define DIV_MIN         1U
#define DIV_MAX         255U      /* integer part of the PWM divider is 8 bits */

uint8_t fan_pwm_div(uint32_t clk_hz, uint32_t freq_hz)
{
    const uint32_t max_ticks = FAN_PWM_TOP_MAX + 1U;    /* per cycle */
    uint32_t ticks;
    uint32_t div;
 
    if (freq_hz == 0U) {
        return (uint8_t)DIV_MAX;
    }

    ticks = clk_hz / freq_hz;                           /* at div 1 */
    div = (ticks + max_ticks - 1U) / max_ticks;         /* round up */

    if (div < DIV_MIN) {
        div = DIV_MIN;
    } else if (div > DIV_MAX) {
        div = DIV_MAX;
    } else {
        /* in range */
    }
    return (uint8_t)div;
}

uint16_t fan_pwm_top(uint32_t clk_hz, uint8_t div, uint32_t freq_hz)
{
    uint32_t ticks;

    if (freq_hz == 0U) {
        return (uint16_t)FAN_PWM_TOP_MAX;
    }
    if (div == 0U) {
        div = 1U;
    }

    /* ticks per PWM cycle = TOP + 1. Two divides instead of
     * clk / (div * freq) so the product can't overflow. */
    ticks = (clk_hz / freq_hz) / div;

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
