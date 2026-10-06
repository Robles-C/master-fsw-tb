# Pico 2 bridge firmware

Bare-metal C (Pico SDK, no RTOS) for the RP2350 that sits between the
PolarFire (F Prime) and the motor, servo, fan, encoder and TMP36. Commands
and telemetry travel over RS-485.

## Build and flash

Requires `PICO_SDK_PATH` to point at a Pico SDK (2.x).

```
cmake -B build && cmake --build build -j
```

Hold BOOTSEL while plugging in USB, then copy `build/pico2_bridge.uf2` to the
drive. Debug output is on USB serial; UART0 is reserved for RS-485.

## Host unit tests

```
cd test
cmake -B build && cmake --build build && ctest --test-dir build
```

Tests use the vendored Unity framework (`test/unity/`, v2.6.1) and fake
drivers in `test/fakes/`. They build with the native compiler and do not
need the Pico SDK.
