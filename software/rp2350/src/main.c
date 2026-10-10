/**
 * @file main.c
 * @brief Init calls and the non-blocking superloop. Nothing else.
 */
#include <stdbool.h>
#include <stdio.h>
#include <stdint.h>

#include "pico/stdlib.h"

#include "board_pins.h"
#include "config.h"
#include "drivers/fan.h"
#include "drivers/servo.h"
#include "drivers/tmp36.h"

/* Temporary bring-up step sizes for the keyboard controls */
#define FAN_BRINGUP_STEP_PCT        5U
#define SERVO_BRINGUP_STEP_DEG      10U
#define SERVO_BRINGUP_FINE_DEG      1U

int main(void)
{
    (void)stdio_init_all();

    gpio_init(PIN_LED);
    gpio_set_dir(PIN_LED, GPIO_OUT);
    tmp36_init();
    fan_init();
    servo_init();

    bool led_on = false;
    uint32_t last_heartbeat_ms = to_ms_since_boot(get_absolute_time());


    uint8_t fan_pct = 0U;
    uint16_t servo_deg = SERVO_RANGE_DEG / 2U;

    for (;;) {
        const uint32_t now_ms = to_ms_since_boot(get_absolute_time());

        /* Unsigned subtraction handles the 32-bit ms wraparound. */
        if ((now_ms - last_heartbeat_ms) >= HEARTBEAT_PERIOD_MS) {
            last_heartbeat_ms += HEARTBEAT_PERIOD_MS;
            led_on = !led_on;
            gpio_put(PIN_LED, led_on);
        }

        /* Temporary bring-up keys. Returns PICO_ERROR_TIMEOUT at once if
         * no key is waiting.
         *   fan:   '+'/'-' step, '0' off, 'f' full
         *   servo: 'r' right (0 deg), 'c' center, 'l' left (max deg),
         *          ']'/'[' +/-10 deg, '}'/'{' +/-1 deg */
        const int key = getchar_timeout_us(0U);
        if (key != PICO_ERROR_TIMEOUT) {
            bool fan_changed = true;
            bool servo_changed = true;

            /* Below FAN_MIN_DUTY_PCT is off: '+' from off jumps to the
             * minimum, '-' from the minimum goes to off. */
            if ((key == '+') && (fan_pct < FAN_MIN_DUTY_PCT)) {
                fan_pct = FAN_MIN_DUTY_PCT;
            } else if ((key == '+') &&
                       (fan_pct <= (100U - FAN_BRINGUP_STEP_PCT))) {
                fan_pct += FAN_BRINGUP_STEP_PCT;
            } else if ((key == '-') &&
                       (fan_pct < (FAN_MIN_DUTY_PCT + FAN_BRINGUP_STEP_PCT))) {
                fan_pct = 0U;
            } else if (key == '-') {
                fan_pct -= FAN_BRINGUP_STEP_PCT;
            } else if (key == '0') {
                fan_pct = 0U;
            } else if (key == 'f') {
                fan_pct = 100U;
            } else {
                fan_changed = false;
            }

            if ((key == ']') &&
                (servo_deg <= (SERVO_RANGE_DEG - SERVO_BRINGUP_STEP_DEG))) {
                servo_deg += SERVO_BRINGUP_STEP_DEG;
            } else if ((key == '[') && (servo_deg >= SERVO_BRINGUP_STEP_DEG)) {
                servo_deg -= SERVO_BRINGUP_STEP_DEG;
            } else if ((key == '}') &&
                       (servo_deg <= (SERVO_RANGE_DEG - SERVO_BRINGUP_FINE_DEG))) {
                servo_deg += SERVO_BRINGUP_FINE_DEG;
            } else if ((key == '{') && (servo_deg >= SERVO_BRINGUP_FINE_DEG)) {
                servo_deg -= SERVO_BRINGUP_FINE_DEG;
            } else if (key == 'r') {
                servo_deg = 0U;
            } else if (key == 'c') {
                servo_deg = SERVO_RANGE_DEG / 2U;
            } else if (key == 'l') {
                servo_deg = SERVO_RANGE_DEG;
            } else {
                servo_changed = false;
            }

            if (fan_changed) {
                fan_set_percent(fan_pct);
                printf("fan -> %u %%\n", (unsigned)fan_pct);
            }
            if (servo_changed) {
                servo_set_angle_deg(servo_deg);
                printf("servo -> %u deg (%u us)\n", (unsigned)servo_deg,
                       (unsigned)servo_angle_to_pulse_us(servo_deg,
                                                         SERVO_MIN_PULSE_US,
                                                         SERVO_MAX_PULSE_US,
                                                         SERVO_RANGE_DEG));
            }
        }
    }

    return 0;
}
