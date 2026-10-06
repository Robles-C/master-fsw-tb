/**
 * @file board_pins.h
 * @brief GPIO assignments for the Pico 2 bridge board.
 *
 * This is the only file that names GPIO numbers. PWM slices and channels
 * are fixed by the GPIO (slice = (gpio >> 1) & 7, channel A = even, B = odd);
 * drivers derive them with pwm_gpio_to_slice_num() and the expected values
 * are noted here for reference only.
 */
#ifndef BOARD_PINS_H
#define BOARD_PINS_H

/* Onboard LED (heartbeat) */
#define PIN_LED             25U

/* RS-485 transceiver (MAX485) on UART0 */
#define PIN_RS485_TX        0U
#define PIN_RS485_RX        1U
#define PIN_RS485_DE_RE     2U   /* high = drive bus, low = receive */

/* Motor driver (DRV8871), PWM slice 3 */
#define PIN_MOTOR_IN2       6U   /* slice 3, channel A */
#define PIN_MOTOR_IN1       7U   /* slice 3, channel B */

/* Quadrature encoder, read with PIO. B must be A + 1. */
#define PIN_ENCODER_A       16U
#define PIN_ENCODER_B       17U

/* Servo signal, PWM slice 1 */
#define PIN_SERVO           18U  /* slice 1, channel A */

/* Fan low-side MOSFET gate, PWM slice 2 */
#define PIN_FAN             20U  /* slice 2, channel A */

/* TMP36 analog output */
#define PIN_TMP36           26U
#define ADC_CH_TMP36        0U   /* GP26 = ADC0 */

#endif /* BOARD_PINS_H */
