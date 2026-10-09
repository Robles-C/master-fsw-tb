/**
 * @file test_servo.c
 * @brief Unit tests for the servo divider, TOP, and angle-to-pulse math.
 */
#include "unity.h"

#include "drivers/servo.h"

#define CLK_HZ      150000000U
#define MIN_US      500U
#define MAX_US      2500U
#define RANGE_DEG   180U

void setUp(void) {}
void tearDown(void) {}

/* 150 MHz / 150 = 1 MHz -> 1 us per tick */
static void test_clkdiv_150mhz(void)
{
    TEST_ASSERT_EQUAL_UINT8(150, servo_clkdiv(CLK_HZ));
}

/* 1 MHz / 50 Hz = 20000 ticks per cycle -> TOP 19999 */
static void test_top_50hz(void)
{
    TEST_ASSERT_EQUAL_UINT16(19999, servo_pwm_top(50U));
}

static void test_pulse_0_deg(void)
{
    TEST_ASSERT_EQUAL_UINT16(500, servo_angle_to_pulse_us(0U, MIN_US, MAX_US, RANGE_DEG));
}

static void test_pulse_45_deg(void)
{
    TEST_ASSERT_EQUAL_UINT16(1000, servo_angle_to_pulse_us(45U, MIN_US, MAX_US, RANGE_DEG));
}

static void test_pulse_90_deg_is_center(void)
{
    TEST_ASSERT_EQUAL_UINT16(1500, servo_angle_to_pulse_us(90U, MIN_US, MAX_US, RANGE_DEG));
}

static void test_pulse_180_deg(void)
{
    TEST_ASSERT_EQUAL_UINT16(2500, servo_angle_to_pulse_us(180U, MIN_US, MAX_US, RANGE_DEG));
}

static void test_pulse_above_range_clamps(void)
{
    TEST_ASSERT_EQUAL_UINT16(2500, servo_angle_to_pulse_us(200U, MIN_US, MAX_US, RANGE_DEG));
}

/* 179 * 2000 / 180 = 1988.9 -> 1988. Divide-before-multiply would give
 * (2000 / 180) * 179 = 11 * 179 = 1969 -> 2469. */
static void test_pulse_179_deg_multiply_first(void)
{
    TEST_ASSERT_EQUAL_UINT16(2488, servo_angle_to_pulse_us(179U, MIN_US, MAX_US, RANGE_DEG));
}

/* Different range than config.h: the function must use its parameters */
static void test_pulse_other_range(void)
{
    TEST_ASSERT_EQUAL_UINT16(1500, servo_angle_to_pulse_us(45U, 1000U, 2000U, 90U));
}

int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_clkdiv_150mhz);
    RUN_TEST(test_top_50hz);
    RUN_TEST(test_pulse_0_deg);
    RUN_TEST(test_pulse_45_deg);
    RUN_TEST(test_pulse_90_deg_is_center);
    RUN_TEST(test_pulse_180_deg);
    RUN_TEST(test_pulse_above_range_clamps);
    RUN_TEST(test_pulse_179_deg_multiply_first);
    RUN_TEST(test_pulse_other_range);
    return UNITY_END();
}
