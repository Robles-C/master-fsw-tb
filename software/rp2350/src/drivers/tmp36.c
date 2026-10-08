/**
 * @file tmp36.c
 * @brief TMP36 hardware wrapper: ADC setup and sampling.
 */
#include <stdint.h>

#include "hardware/adc.h"

#include "board_pins.h"
#include "drivers/tmp36.h"

void tmp36_init(void)
{
    adc_init();
    adc_gpio_init(PIN_TMP36);
}

float tmp36_read_c(void)
{
    adc_select_input(ADC_CH_TMP36);

    const uint16_t counts = adc_read();

    return tmp36_counts_to_c(counts);
}
