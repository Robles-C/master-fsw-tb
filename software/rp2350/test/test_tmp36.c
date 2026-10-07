/**
 * @file test_tmp36.c
 * @brief Unit tests for the TMP36 counts-to-Celsius conversion.
 */
#include "unity.h"

#include "drivers/tmp36.h"

void setUp(void) {}
void tearDown(void) {}

static void test_25c(void)
{
    TEST_ASSERT_FLOAT_WITHIN(0.1f, 25.0f, tmp36_counts_to_c(931));
}

static void test_0c(void)
{
    TEST_ASSERT_FLOAT_WITHIN(0.1f, 0.0f, tmp36_counts_to_c(621));
}

static void test_1000_counts(void)
{
    TEST_ASSERT_FLOAT_WITHIN(0.1f, 30.6f, tmp36_counts_to_c(1000));
}

static void test_negative_temp(void)
{
    TEST_ASSERT_FLOAT_WITHIN(0.1f, -10.0f, tmp36_counts_to_c(496));
}

int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_25c);
    RUN_TEST(test_0c);
    RUN_TEST(test_1000_counts);
    RUN_TEST(test_negative_temp);
    return UNITY_END();
}