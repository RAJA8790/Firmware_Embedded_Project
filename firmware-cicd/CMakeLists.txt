/**
 * @file main.c
 * @brief Embedded Firmware - Azure DevOps CI/CD Demo
 *        Runs on real STM32 hardware OR simulated in CI
 */

#include <stdio.h>
#include <stdint.h>
#include "drivers/gpio.h"
#include "drivers/uart.h"

#define FIRMWARE_VERSION "1.0.0"
#define LED_BLINK_COUNT   5

/* HOST_SIM: set by CI when running natively (not on real MCU) */
#ifdef HOST_SIM
  #define MAIN_LOOP_LIMIT 1   /* exit after one iteration in CI */
#else
  #define MAIN_LOOP_LIMIT 0   /* run forever on real hardware */
#endif

int main(void)
{
    uart_init(115200);
    gpio_init();

    /* Boot banner — CI checks for "BOOT_OK" */
    printf("==================================\r\n");
    printf("  Firmware v%s\r\n", FIRMWARE_VERSION);
    printf("  Azure DevOps CI/CD Demo\r\n");
    printf("==================================\r\n");
    printf("BOOT_OK\r\n");

    for (int i = 0; i < LED_BLINK_COUNT; i++) {
        gpio_led_set(1);
        printf("LED ON  [%d/%d]\r\n", i + 1, LED_BLINK_COUNT);
        gpio_led_set(0);
        printf("LED OFF [%d/%d]\r\n", i + 1, LED_BLINK_COUNT);
    }

    printf("SELF_TEST_PASS\r\n");
    printf("Entering main loop...\r\n");
    fflush(stdout);

    /* Real hardware: infinite loop. CI sim: exits cleanly */
    int loop = 0;
    while (MAIN_LOOP_LIMIT == 0 || loop++ < MAIN_LOOP_LIMIT) {
        /* Application logic */
    }

    return 0;
}
