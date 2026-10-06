/**
 * @file test_smoke.c
 * @brief Checks that the host test harness builds and runs, and that the
 *        shared headers compile without the Pico SDK.
 */
#include "unity.h"

#include "board_pins.h"
#include "config.h"

void setUp(void) {}
void tearDown(void) {}

static void test_encoder_pins_are_consecutive(void)
{
    /* The quadrature PIO program reads A and B as a pin pair. */
    TEST_ASSERT_EQUAL_UINT32(PIN_ENCODER_A + 1U, PIN_ENCODER_B);
}

static void test_tmp36_is_on_adc0(void)
{
    /* ADC channel n is GP(26 + n) on the RP2350A. */
    TEST_ASSERT_EQUAL_UINT32(PIN_TMP36, 26U + ADC_CH_TMP36);
}

int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_encoder_pins_are_consecutive);
    RUN_TEST(test_tmp36_is_on_adc0);
    return UNITY_END();
}
