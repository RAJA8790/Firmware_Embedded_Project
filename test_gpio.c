cmake_minimum_required(VERSION 3.16)
project(embedded_firmware_cicd C)
set(CMAKE_C_STANDARD 11)

option(HOST     "Build for host (unit tests)" OFF)
option(COVERAGE "Enable gcov code coverage"   OFF)

if(HOST)
    message(STATUS "Build mode: HOST (unit tests)")

    if(COVERAGE)
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} --coverage -fprofile-arcs -ftest-coverage")
        set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} --coverage")
    endif()

    include(CTest)
    enable_testing()

    set(DRIVER_SOURCES src/drivers/gpio.c src/drivers/uart.c)

    add_executable(test_gpio tests/test_gpio.c ${DRIVER_SOURCES})
    target_include_directories(test_gpio PRIVATE src lib/Unity)
    add_test(NAME GPIO_Tests COMMAND test_gpio)

    add_executable(test_uart tests/test_uart.c ${DRIVER_SOURCES})
    target_include_directories(test_uart PRIVATE src lib/Unity)
    add_test(NAME UART_Tests COMMAND test_uart)

else()
    message(STATUS "Build mode: ARM firmware")
    set(CMAKE_SYSTEM_NAME Generic)
    set(CMAKE_SYSTEM_PROCESSOR arm)
    set(CMAKE_C_COMPILER arm-none-eabi-gcc)
    set(CMAKE_OBJCOPY arm-none-eabi-objcopy)
    set(CPU_FLAGS "-mcpu=cortex-m4 -mthumb -mfloat-abi=soft")
    set(CMAKE_C_FLAGS "${CPU_FLAGS} -O2 -Wall -Wextra -ffunction-sections -fdata-sections")
    set(CMAKE_EXE_LINKER_FLAGS "-T ${CMAKE_SOURCE_DIR}/linker.ld -nostartfiles -Wl,--gc-sections")

    add_executable(firmware.elf src/main.c src/drivers/gpio.c src/drivers/uart.c)
    target_include_directories(firmware.elf PRIVATE src)

    add_custom_command(TARGET firmware.elf POST_BUILD
        COMMAND ${CMAKE_OBJCOPY} -O binary firmware.elf firmware.bin
        COMMAND ${CMAKE_OBJCOPY} -O ihex   firmware.elf firmware.hex
        COMMENT "Generating firmware.bin and firmware.hex"
    )
endif()
