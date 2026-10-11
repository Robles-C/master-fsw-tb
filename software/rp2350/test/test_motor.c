/**
 * @file test_motor.c
 * @brief Unit tests for the motor TOP and command-to-levels (slow decay) math.
 */
#include "unity.h"

#include "drivers/motor.h"

#define CLK_HZ      150000000U
#define TOP_20K     7499U
#define FULL_20K    7500U       /* TOP + 1: pin always high */

void setUp(void) {}
void tearDown(void) {}

static void check_levels(int8_t cmd, uint16_t in1, uint16_t in2)
{
    const motor_levels_t lv = motor_cmd_to_levels(cmd, TOP_20K);

    TEST_ASSERT_EQUAL_UINT16(in1, lv.in1);
    TEST_ASSERT_EQUAL_UINT16(in2, lv.in2);
}

/* 150 MHz / 20 kHz = 7500 ticks per cycle -> TOP 7499 */
static void test_top_20khz(void)
{
    TEST_ASSERT_EQUAL_UINT16(7499, motor_pwm_top(CLK_HZ, 20000U));
}

static void test_top_zero_freq_clamps(void)
{
    TEST_ASSERT_EQUAL_UINT16(MOTOR_PWM_TOP_MAX, motor_pwm_top(CLK_HZ, 0U));
}

/* 0 = coast: both inputs low */
static void test_zero_coasts(void)
{
    check_levels(0, 0U, 0U);
}

/* Full forward: IN1 high, IN2 never brakes */
static void test_forward_100(void)
{
    check_levels(100, FULL_20K, 0U);
}

/* Drive 3750 ticks, brake 3750 ticks */
static void test_forward_50(void)
{
    check_levels(50, FULL_20K, 3750U);
}

/* Drive 1875 ticks, brake 5625 ticks (75 % of the cycle) */
static void test_forward_25(void)
{
    check_levels(25, FULL_20K, 5625U);
}

/* Drive 33 * 7500 / 100 = 2475 -> brake 5025. Computing 33 / 100 first
 * would give drive 0 -> brake 7500. */
static void test_forward_33_multiply_first(void)
{
    check_levels(33, FULL_20K, 5025U);
}

/* Reverse mirrors forward: IN2 high, IN1 brakes */
static void test_reverse_50(void)
{
    check_levels(-50, 3750U, FULL_20K);
}

static void test_reverse_100(void)
{
    check_levels(-100, 0U, FULL_20K);
}

static void test_above_100_clamps(void)
{
    check_levels(127, FULL_20K, 0U);
}

/* -128 has no positive int8_t: negating it carelessly overflows */
static void test_below_minus_100_clamps(void)
{
    check_levels(-128, 0U, FULL_20K);
}

int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_top_20khz);
    RUN_TEST(test_top_zero_freq_clamps);
    RUN_TEST(test_zero_coasts);
    RUN_TEST(test_forward_100);
    RUN_TEST(test_forward_50);
    RUN_TEST(test_forward_25);
    RUN_TEST(test_forward_33_multiply_first);
    RUN_TEST(test_reverse_50);
    RUN_TEST(test_reverse_100);
    RUN_TEST(test_above_100_clamps);
    RUN_TEST(test_below_minus_100_clamps);
    return UNITY_END();
}
