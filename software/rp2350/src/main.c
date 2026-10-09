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
#include "drivers/tmp36.h"

/* Temporary fan bring-up: step size for the +/- keys */
#define FAN_BRINGUP_STEP_PCT    5U

int main(void)
{
    (void)stdio_init_all();

    gpio_init(PIN_LED);
    gpio_set_dir(PIN_LED, GPIO_OUT);
    tmp36_init();
    fan_init();

    bool led_on = false;
    uint32_t last_heartbeat_ms = to_ms_since_boot(get_absolute_time());

    uint32_t last_tmp_print_ms = to_ms_since_boot(get_absolute_time());

    uint8_t fan_pct = 0U;

    for (;;) {
        const uint32_t now_ms = to_ms_since_boot(get_absolute_time());

        /* Unsigned subtraction handles the 32-bit ms wraparound. */
        if ((now_ms - last_heartbeat_ms) >= HEARTBEAT_PERIOD_MS) {
            last_heartbeat_ms += HEARTBEAT_PERIOD_MS;
            led_on = !led_on;
            gpio_put(PIN_LED, led_on);
        }

        if ((now_ms - last_tmp_print_ms) >= TMP_PRINT_PERIOD_MS) {
            last_tmp_print_ms += TMP_PRINT_PERIOD_MS;
            printf("tmp: %.2f C  fan: %u %%\n", (double)tmp36_read_c(),
                   (unsigned)fan_pct);
        }

        /* Temporary fan bring-up: '+'/'-' step, '0' off, 'f' full.
         * Returns PICO_ERROR_TIMEOUT at once if no key is waiting. */
        const int key = getchar_timeout_us(0U);
        if (key != PICO_ERROR_TIMEOUT) {
            if ((key == '+') && (fan_pct <= (100U - FAN_BRINGUP_STEP_PCT))) {
                fan_pct += FAN_BRINGUP_STEP_PCT;
            } else if ((key == '-') && (fan_pct >= FAN_BRINGUP_STEP_PCT)) {
                fan_pct -= FAN_BRINGUP_STEP_PCT;
            } else if (key == '0') {
                fan_pct = 0U;
            } else if (key == 'f') {
                fan_pct = 100U;
            } else {
                /* ignore other keys */
            }
            fan_set_percent(fan_pct);
            printf("fan -> %u %%\n", (unsigned)fan_pct);
        }
    }

    return 0;
}
