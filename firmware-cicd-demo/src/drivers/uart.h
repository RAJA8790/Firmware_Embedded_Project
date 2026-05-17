#ifndef UART_H
#define UART_H

#include <stdint.h>

/**
 * @brief Initialize UART with given baud rate
 * @param baud Baud rate (e.g. 115200)
 */
void uart_init(uint32_t baud);

/**
 * @brief Transmit a single byte
 * @param byte Data to send
 */
void uart_transmit(uint8_t byte);

/**
 * @brief Receive a single byte (blocking)
 * @return Received byte
 */
uint8_t uart_receive(void);

/**
 * @brief Transmit a null-terminated string
 * @param str Pointer to string
 */
void uart_print(const char *str);

/**
 * @brief Check if data is available in RX buffer
 * @return 1 if data available, 0 otherwise
 */
uint8_t uart_data_available(void);

#endif /* UART_H */
