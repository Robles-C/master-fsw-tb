/**
 * @file servo.c
 * @brief Servo hardware wrapper: PWM slice setup and pulse updates.
 */
#include <stdint.h>

#include "hardware/clocks.h"
#include "hardware/gpio.h"
#include "hardware/pwm.h"

#include "board_pins.h"
#include "config.h"
#include "drivers/servo.h"

void servo_init(void)
{
    const uint32_t slice = pwm_gpio_to_slice_num(PIN_SERVO);
    pwm_config cfg = pwm_get_default_config();

    pwm_config_set_clkdiv_int(&cfg, servo_clkdiv(clock_get_hz(clk_sys)));
    pwm_config_set_wrap(&cfg, servo_pwm_top(SERVO_PWM_FREQ_HZ));

    /* Level 0 = pin held low, no pulse: servo stays where it is until the
     * first servo_set_angle_deg() */
    pwm_init(slice, &cfg, false);
    pwm_set_gpio_level(PIN_SERVO, 0U);
    gpio_set_function(PIN_SERVO, GPIO_FUNC_PWM);
    pwm_set_enabled(slice, true);
}

void servo_set_angle_deg(uint16_t angle_deg)
{
    /* 1 tick = 1 us, so the pulse width in us is the level */
    pwm_set_gpio_level(PIN_SERVO,
                       servo_angle_to_pulse_us(angle_deg, SERVO_MIN_PULSE_US,
                                               SERVO_MAX_PULSE_US,
                                               SERVO_RANGE_DEG));
}
