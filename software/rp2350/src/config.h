/**
 * @file config.h
 * @brief Tunable constants: baud, PWM frequencies, loop rates, thresholds.
 */
#ifndef CONFIG_H
#define CONFIG_H

/* Loop rates */
#define HEARTBEAT_PERIOD_MS     500U

/* RS-485 */
#define RS485_BAUD              115200U

/* PWM frequencies */
#define MOTOR_PWM_FREQ_HZ       20000U
#define SERVO_PWM_FREQ_HZ       50U
#define FAN_PWM_FREQ_HZ         25000U

#endif /* CONFIG_H */
