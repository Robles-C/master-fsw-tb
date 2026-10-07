/**
 * @file tmp36_convert.c
 * @brief Pure TMP36 conversion. No hardware access, builds on the host.
 */
#include <stdint.h>

#include "drivers/tmp36.h"

#define ADC_VREF_V          3.3f     /* ADC reference voltage */
#define ADC_FULL_SCALE      4096.0f  /* 12-bit ADC: 2^12 steps */
#define TMP36_OFFSET_V      0.5f     /* output at 0 C */
#define TMP36_V_PER_C       0.01f    /* 10 mV per degree */

float tmp36_counts_to_c(uint16_t counts)
{
    const float volts = ((float)counts * ADC_VREF_V) / ADC_FULL_SCALE;

    return (volts - TMP36_OFFSET_V) / TMP36_V_PER_C;
}