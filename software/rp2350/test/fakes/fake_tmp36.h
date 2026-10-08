/**
 * @file fake_tmp36.h
 * @brief Test controls for the fake TMP36 driver.
 *
 * fake_tmp36.c implements drivers/tmp36.h without any hardware. Tests use
 * these functions to choose what tmp36_read_c() returns and to check how
 * the code under test used the driver.
 */
#ifndef FAKE_TMP36_H
#define FAKE_TMP36_H

#include <stdbool.h>
#include <stdint.h>

/* clears all fake state. Call from setUp() so tests don't leak into each other */
void fake_tmp36_reset(void);

/* sets the temperature the next tmp36_read_c() calls will return */
void fake_tmp36_set_c(float temp_c);

/* true once tmp36_init() has been called since the last reset */
bool fake_tmp36_init_called(void);

/* num of tmp36_read_c() calls since the last reset */
uint32_t fake_tmp36_read_count(void);

#endif /* FAKE_TMP36_H */
