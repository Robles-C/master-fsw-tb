cmake_minimum_required(VERSION 3.13)

# Host-side unit tests. Builds with the native compiler, no Pico SDK.
project(rp2350_tests C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

enable_testing()

set(SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../src)

add_library(unity STATIC unity/unity.c)
target_include_directories(unity PUBLIC unity)

# add_unit_test(<name> <sources>...)
# Builds test_<name>.c plus the given sources (module under test and fakes)
# into one executable and registers it with ctest.
function(add_unit_test name)
    add_executable(test_${name} test_${name}.c ${ARGN})
    target_include_directories(test_${name} PRIVATE ${SRC_DIR} fakes)
    target_compile_options(test_${name} PRIVATE -Wall -Wextra -Werror)
    target_link_libraries(test_${name} PRIVATE unity m)
    add_test(NAME ${name} COMMAND test_${name})
endfunction()

add_unit_test(smoke)
add_unit_test(tmp36 ${SRC_DIR}/drivers/tmp36_convert.c)
