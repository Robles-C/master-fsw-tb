/**
 * @file motor.c
 * @brief Motor hardware wrapper: PWM slice setup and IN1/IN2 updates.
 *
 * IN1 and IN2 must be the two channels of one PWM slice (see board_pins.h).
 */
#include <stdint.h>

#include "hardware/clocks.h"
#include "hardware/gpio.h"
#include "hardware/pwm.h"

#include "board_pins.h"
#include "config.h"
#include "drivers/motor.h"

/* TOP computed at init, needed to convert commands to levels */
static uint16_t s_top;

/* Writes both inputs in one register write, so the bridge never runs a PWM
 * cycle with one new level and one old one. */
static void write_levels(motor_levels_t lv)
{
    const uint32_t slice = pwm_gpio_to_slice_num(PIN_MOTOR_IN1);

    if (pwm_gpio_to_channel(PIN_MOTOR_IN1) == PWM_CHAN_A) {
        pwm_set_both_levels(slice, lv.in1, lv.in2);
    } else {
        pwm_set_both_levels(slice, lv.in2, lv.in1);
    }
}

void motor_init(void)
{
    const uint32_t slice = pwm_gpio_to_slice_num(PIN_MOTOR_IN1);
    pwm_config cfg = pwm_get_default_config();

    s_top = motor_pwm_top(clock_get_hz(clk_sys), MOTOR_PWM_FREQ_HZ);

    pwm_config_set_clkdiv_int(&cfg, 1U);
    pwm_config_set_wrap(&cfg, s_top);

    /* Both levels 0 (coast) before the pins are handed to PWM */
    pwm_init(slice, &cfg, false);
    write_levels(motor_cmd_to_levels(0, s_top));
    gpio_set_function(PIN_MOTOR_IN1, GPIO_FUNC_PWM);
    gpio_set_function(PIN_MOTOR_IN2, GPIO_FUNC_PWM);
    pwm_set_enabled(slice, true);
}

void motor_set(int8_t cmd)
{
    write_levels(motor_cmd_to_levels(cmd, s_top));
}

void motor_brake(void)
{
    const motor_levels_t brake = { (uint16_t)(s_top + 1U),
                                   (uint16_t)(s_top + 1U) };

    write_levels(brake);
}
