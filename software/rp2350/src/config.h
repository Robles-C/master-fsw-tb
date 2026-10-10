/**
 * @file config.h
 * @brief Tunable constants: baud, PWM frequencies, loop rates, thresholds.
 */
#ifndef CONFIG_H
#define CONFIG_H

/* Loop rates */
#define HEARTBEAT_PERIOD_MS     500U

/* Print rate*/
#define TMP_PRINT_PERIOD_MS     1000U

/* RS-485 */
#define RS485_BAUD              115200U

/* PWM frequencies */
#define MOTOR_PWM_FREQ_HZ       20000U
#define SERVO_PWM_FREQ_HZ       50U
#define FAN_PWM_FREQ_HZ         10U     /* slow PWM: 2-wire fan stalled at 25 kHz */

/* Fan minimum running duty: callers treat anything lower as off */
#define FAN_MIN_DUTY_PCT        50U

/* Servo pulse range and travel */
#define SERVO_MIN_PULSE_US      700U    /* pulse at 0 deg (rightmost) */
#define SERVO_MAX_PULSE_US      1653U   /* pulse at SERVO_RANGE_DEG (leftmost) */
#define SERVO_RANGE_DEG         180U    /* linear: 90 deg = 1176 us (center) */

#endif /* CONFIG_H */
