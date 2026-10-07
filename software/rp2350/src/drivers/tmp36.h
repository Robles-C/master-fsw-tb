/**
 * @file tmp36.h
 * @brief TMP36 analog temperature sensor on the RP2350 ADC.
 *
 * Temperatures are floats in degrees Celsius, so 25.37 C is 25.37.
 * Values can be negative (the sensor reads down to -40 C).
 */
#ifndef TMP36_H
#define TMP36_H

#include <stdint.h>

/* Configures the ADC and the TMP36 pin at startup */
void tmp36_init(void);

/* Samples TMP36 once and returns temp in degrees C*/
float tmp36_read_c(void);

/* Converts 12 bit ADC reading to degrees C. No hardware access */
float tmp36_counts_to_c(uint16_t counts);

#endif /* TMP36_H */
