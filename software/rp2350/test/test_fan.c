/**
 * @file test_fan.c
 * @brief Unit tests for the fan PWM divider, TOP, and percent-to-level math.
 */
#include "unity.h"

#include "drivers/fan.h"

#define CLK_HZ  150000000U
#define TOP_25K 5999U
#define TOP_25HZ 65216U

void setUp(void) {}
void tearDown(void) {}

/* 6000 ticks per cycle already fits in 16 bits -> div 1 */
static void test_div_25khz_is_1(void)
{
    TEST_ASSERT_EQUAL_UINT8(1, fan_pwm_div(CLK_HZ, 25000U));
}

/* 6,000,000 ticks / 65535 = 91.6 -> round up to 92 */
static void test_div_25hz(void)
{
    TEST_ASSERT_EQUAL_UINT8(92, fan_pwm_div(CLK_HZ, 25U));
}

/* 1 Hz would need div 2289 -> clamped to the 8-bit max */
static void test_div_too_low_freq_clamps(void)
{
    TEST_ASSERT_EQUAL_UINT8(255, fan_pwm_div(CLK_HZ, 1U));
}

static void test_div_zero_freq_clamps(void)
{
    TEST_ASSERT_EQUAL_UINT8(255, fan_pwm_div(CLK_HZ, 0U));
}

/* 150 MHz / 25 kHz = 6000 ticks per cycle -> TOP 5999 */
static void test_top_25khz(void)
{
    TEST_ASSERT_EQUAL_UINT16(5999, fan_pwm_top(CLK_HZ, 1U, 25000U));
}

/* 150 MHz / (92 * 25 Hz) = 65217.4 ticks -> 65217 -> TOP 65216 */
static void test_top_25hz_div_92(void)
{
    TEST_ASSERT_EQUAL_UINT16(TOP_25HZ, fan_pwm_top(CLK_HZ, 92U, 25U));
}

/* 150 MHz / 100 Hz at div 1 = 1.5M ticks, too many for 16 bits -> clamped */
static void test_top_too_low_freq_clamps(void)
{
    TEST_ASSERT_EQUAL_UINT16(FAN_PWM_TOP_MAX, fan_pwm_top(CLK_HZ, 1U, 100U));
}

/* 0 Hz would divide by zero -> clamped */
static void test_top_zero_freq_clamps(void)
{
    TEST_ASSERT_EQUAL_UINT16(FAN_PWM_TOP_MAX, fan_pwm_top(CLK_HZ, 1U, 0U));
}

/* div 0 is treated as 1 */
static void test_top_div_0_as_1(void)
{
    TEST_ASSERT_EQUAL_UINT16(5999, fan_pwm_top(CLK_HZ, 0U, 25000U));
}

static void test_level_0_percent(void)
{
    TEST_ASSERT_EQUAL_UINT16(0, fan_percent_to_level(0U, TOP_25K));
}

static void test_level_50_percent(void)
{
    TEST_ASSERT_EQUAL_UINT16(3000, fan_percent_to_level(50U, TOP_25K));
}

/* 100 % must be TOP+1 so the pin never goes low */
static void test_level_100_percent_is_top_plus_1(void)
{
    TEST_ASSERT_EQUAL_UINT16(6000, fan_percent_to_level(100U, TOP_25K));
}

static void test_level_above_100_clamps(void)
{
    TEST_ASSERT_EQUAL_UINT16(6000, fan_percent_to_level(200U, TOP_25K));
}

/* Catches divide-before-multiply: 33 / 100 would be 0 */
static void test_level_33_percent(void)
{
    TEST_ASSERT_EQUAL_UINT16(1980, fan_percent_to_level(33U, TOP_25K));
}

/* Largest TOP: 100 % is 65535, the top of the 16-bit range */
static void test_level_100_percent_at_max_top(void)
{
    TEST_ASSERT_EQUAL_UINT16(65535, fan_percent_to_level(100U, FAN_PWM_TOP_MAX));
}

/* 50 % of 65217 ticks = 32608.5 -> 32608 */
static void test_level_50_percent_25hz(void)
{
    TEST_ASSERT_EQUAL_UINT16(32608, fan_percent_to_level(50U, TOP_25HZ));
}

int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_div_25khz_is_1);
    RUN_TEST(test_div_25hz);
    RUN_TEST(test_div_too_low_freq_clamps);
    RUN_TEST(test_div_zero_freq_clamps);
    RUN_TEST(test_top_25khz);
    RUN_TEST(test_top_25hz_div_92);
    RUN_TEST(test_top_too_low_freq_clamps);
    RUN_TEST(test_top_zero_freq_clamps);
    RUN_TEST(test_top_div_0_as_1);
    RUN_TEST(test_level_0_percent);
    RUN_TEST(test_level_50_percent);
    RUN_TEST(test_level_100_percent_is_top_plus_1);
    RUN_TEST(test_level_above_100_clamps);
    RUN_TEST(test_level_33_percent);
    RUN_TEST(test_level_100_percent_at_max_top);
    RUN_TEST(test_level_50_percent_25hz);
    return UNITY_END();
}
