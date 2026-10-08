/**
 * @file fake_tmp36.c
 * @brief Host-side stand-in for drivers/tmp36.c. No ADC, no Pico SDK.
 *
 * tmp36_counts_to_c() is not faked: tests link the real tmp36_convert.c.
 */
#include <stdbool.h>
#include <stdint.h>

#include "drivers/tmp36.h"
#include "fake_tmp36.h"

static float    s_temp_c;
static bool     s_init_called;
static uint32_t s_read_count;

void fake_tmp36_reset(void)
{
    s_temp_c = 0.0f;
    s_init_called = false;
    s_read_count = 0U;
}

void fake_tmp36_set_c(float temp_c)
{
    s_temp_c = temp_c;
}

bool fake_tmp36_init_called(void)
{
    return s_init_called;
}

uint32_t fake_tmp36_read_count(void)
{
    return s_read_count;
}

void tmp36_init(void)
{
    s_init_called = true;
}

float tmp36_read_c(void)
{
    s_read_count++;
    return s_temp_c;
}
