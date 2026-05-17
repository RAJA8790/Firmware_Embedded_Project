#include "gpio.h"
#include <stdio.h>
#include <stdint.h>

/* Simulated GPIO register — read and write share same register */
static uint16_t gpio_reg = 0x0000;

void gpio_init(void)
{
    gpio_reg = 0x0000;
}

void gpio_led_set(uint8_t state)
{
    if (state) {
        gpio_reg |=  (1u << 13);
    } else {
        gpio_reg &= ~(1u << 13);
    }
}

uint8_t gpio_read(uint8_t pin)
{
    if (pin > 15) return 0;
    return (gpio_reg >> pin) & 0x01;
}

void gpio_write(uint8_t pin, uint8_t value)
{
    if (pin > 15) return;
    if (value) {
        gpio_reg |=  (1u << pin);
    } else {
        gpio_reg &= ~(1u << pin);
    }
}

void gpio_toggle(uint8_t pin)
{
    if (pin > 15) return;
    gpio_reg ^= (1u << pin);
}
