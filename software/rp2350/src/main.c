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
#include "drivers/tmp36.h"

int main(void)
{
    (void)stdio_init_all();

    gpio_init(PIN_LED);
    gpio_set_dir(PIN_LED, GPIO_OUT);
    tmp36_init();

    bool led_on = false;
    uint32_t last_heartbeat_ms = to_ms_since_boot(get_absolute_time());

    uint32_t last_tmp_print_ms = to_ms_since_boot(get_absolute_time());

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
            printf("tmp: %.2f C\n", (double)tmp36_read_c());
        }
    }

    return 0;
}
