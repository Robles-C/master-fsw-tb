/**
 * @file motor.h
 * @brief Brushed DC motor on a DRV8871 H-bridge, PWM on IN1/IN2.
 *
 * Commands are signed percent: +100 = full forward, -100 = full reverse,
 * 0 = coast. Speed PWM uses slow decay: the off part of each cycle brakes
 * (IN1 = IN2 = 1) instead of coasting, so speed tracks the command closely.
 */
#ifndef MOTOR_H
#define MOTOR_H

#include <stdint.h>

/* Largest TOP motor_pwm_top() returns, so that level TOP+1 (always high)
 * fits in the 16-bit level register. */
#define MOTOR_PWM_TOP_MAX   65534U

/* PWM levels for the two DRV8871 inputs */
typedef struct {
    uint16_t in1;
    uint16_t in2;
} motor_levels_t;

/* Configures IN1/IN2 for PWM at MOTOR_PWM_FREQ_HZ and starts coasting */
void motor_init(void);

/* Drives the motor at cmd percent, -100 to 100 (clamped outside that).
 * 0 coasts. */
void motor_set(int8_t cmd);

/* Stops hard: shorts the motor through the bridge (IN1 = IN2 = 1).
 * Stays braked until the next motor_set(). */
void motor_brake(void);

/* Returns the PWM TOP for divider 1: clk_hz / freq_hz - 1.
 * Clamped to MOTOR_PWM_TOP_MAX if freq_hz is too low (or 0).
 * No hardware access. */
uint16_t motor_pwm_top(uint32_t clk_hz, uint32_t freq_hz);

/* Converts a command (-100 to 100, clamped) to IN1/IN2 levels for the given
 * TOP, slow decay. With full = top + 1 and drive = |cmd| * full / 100
 * (rounded down):
 *   cmd > 0:  in1 = full,         in2 = full - drive
 *   cmd < 0:  in1 = full - drive, in2 = full
 *   cmd = 0:  in1 = 0,            in2 = 0          (coast)
 * No hardware access. */
motor_levels_t motor_cmd_to_levels(int8_t cmd, uint16_t top);

#endif /* MOTOR_H */
