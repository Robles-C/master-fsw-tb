/**
 * @file servo.h
 * @brief Hobby servo on PWM at 50 Hz, position set by pulse width.
 *
 * Angles are whole degrees from 0 to SERVO_RANGE_DEG (config.h). The PWM
 * counter ticks at 1 MHz, so one tick is 1 us and the PWM level equals the
 * pulse width in microseconds.
 */
#ifndef SERVO_H
#define SERVO_H

#include <stdint.h>

/* Counter tick rate the servo PWM runs at: 1 MHz = 1 us per tick */
#define SERVO_TICK_HZ       1000000U

/* Configures the servo pin for PWM at SERVO_PWM_FREQ_HZ. No pulse is sent
 * until the first servo_set_angle_deg(), so the servo doesn't jump at boot. */
void servo_init(void);

/* Moves the servo to angle_deg. Values above SERVO_RANGE_DEG are clamped. */
void servo_set_angle_deg(uint16_t angle_deg);

/* Returns the integer clock divider that gives SERVO_TICK_HZ from clk_hz
 * (150 MHz -> 150). clk_hz should be a whole number of MHz.
 * No hardware access. */
uint8_t servo_clkdiv(uint32_t clk_hz);

/* Returns the PWM TOP for freq_hz at SERVO_TICK_HZ (50 Hz -> 19999).
 * No hardware access. */
uint16_t servo_pwm_top(uint32_t freq_hz);

/* Converts an angle to a pulse width in us, linear from min_us at 0 deg to
 * max_us at range_deg. angle_deg above range_deg is clamped. Rounds down.
 * No hardware access. */
uint16_t servo_angle_to_pulse_us(uint16_t angle_deg, uint16_t min_us,
                                 uint16_t max_us, uint16_t range_deg);

#endif /* SERVO_H */
