/**
 * @file fan.c
 * @brief Fan hardware wrapper: PWM slice setup and duty updates.
 */
#include <stdint.h>

#include "hardware/clocks.h"
#include "hardware/gpio.h"
#include "hardware/pwm.h"

#include "board_pins.h"
#include "config.h"
#include "drivers/fan.h"

/* TOP computed at init, needed to convert percent to level */
static uint16_t s_top;

void fan_init(void)
{
    const uint32_t slice = pwm_gpio_to_slice_num(PIN_FAN);
    pwm_config cfg = pwm_get_default_config();

    const uint32_t clk_hz = clock_get_hz(clk_sys);
    const uint8_t div = fan_pwm_div(clk_hz, FAN_PWM_FREQ_HZ);

    s_top = fan_pwm_top(clk_hz, div, FAN_PWM_FREQ_HZ);

    pwm_config_set_clkdiv_int(&cfg, div);
    pwm_config_set_wrap(&cfg, s_top);

    /* init at 0*/
    pwm_init(slice, &cfg, false);
    pwm_set_gpio_level(PIN_FAN, 0U);
    gpio_set_function(PIN_FAN, GPIO_FUNC_PWM);
    pwm_set_enabled(slice, true);
}

void fan_set_percent(uint8_t percent)
{
    pwm_set_gpio_level(PIN_FAN, fan_percent_to_level(percent, s_top));
}
