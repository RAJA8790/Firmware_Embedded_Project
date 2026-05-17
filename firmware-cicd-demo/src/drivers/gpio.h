#ifndef GPIO_H
#define GPIO_H

#include <stdint.h>

/**
 * @brief Initialize GPIO peripheral
 */
void gpio_init(void);

/**
 * @brief Set LED state
 * @param state 1 = ON, 0 = OFF
 */
void gpio_led_set(uint8_t state);

/**
 * @brief Read a digital input pin
 * @param pin Pin number (0–15)
 * @return 1 if HIGH, 0 if LOW
 */
uint8_t gpio_read(uint8_t pin);

/**
 * @brief Write a digital output pin
 * @param pin   Pin number (0–15)
 * @param value 1 = HIGH, 0 = LOW
 */
void gpio_write(uint8_t pin, uint8_t value);

/**
 * @brief Toggle a pin
 * @param pin Pin number (0–15)
 */
void gpio_toggle(uint8_t pin);

#endif /* GPIO_H */
