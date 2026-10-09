/**
 * @file fan.h
 * @brief 2-wire fan switched by a low-side N channel MOSFET via pwm 
 *
 * Speed is a duty cycle in percent: 0 = off, 100 = always on. The fan has
 * no tach, so there is no speed feedback. Below the fan's minimum start duty
 * it will not spin; handling that is the caller's job, not the driver's.
 */
#ifndef FAN_H
#define FAN_H

#include <stdint.h>

/* Largest TOP fan_pwm_top() returns, so that level TOP+1 (100 %) fits in
 * the 16-bit level register. */
#define FAN_PWM_TOP_MAX     65534U

/* Configures the fan pin for PWM at FAN_PWM_FREQ_HZ and starts at 0 % */
void fan_init(void);

/* Sets fan duty in percent. Values above 100 are treated as 100. */
void fan_set_percent(uint8_t percent);

/* Returns the PWM TOP (wrap) value for divider 1: clk_hz / freq_hz - 1.
 * Clamped to FAN_PWM_TOP_MAX if freq_hz is too low (or 0) for 16 bits.
 * No hardware access. */
uint16_t fan_pwm_top(uint32_t clk_hz, uint32_t freq_hz);

/* Converts percent (0-100, clamped above 100) to a PWM level for the given
 * TOP: percent * (top + 1) / 100, so 100 % returns top + 1 (always high).
 * Rounds down. No hardware access. */
uint16_t fan_percent_to_level(uint8_t percent, uint16_t top);

#endif /* FAN_H */
