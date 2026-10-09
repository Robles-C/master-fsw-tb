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
#define FAN_PWM_FREQ_HZ         25000U

/* Servo pulse range and travel. to be confirmed*/
#define SERVO_MIN_PULSE_US      500U    /* pulse at 0 deg */
#define SERVO_MAX_PULSE_US      2500U   /* pulse at SERVO_RANGE_DEG */
#define SERVO_RANGE_DEG         180U

#endif /* CONFIG_H */
