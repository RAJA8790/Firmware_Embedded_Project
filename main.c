#include "uart.h"
#include <stdio.h>
#include <string.h>

/* Simulated UART state */
static uint32_t uart_baud    = 0;
static uint8_t  rx_buffer[256];
static uint8_t  rx_head      = 0;
static uint8_t  rx_tail      = 0;
static uint8_t  uart_ready   = 0;

void uart_init(uint32_t baud)
{
    uart_baud  = baud;
    rx_head    = 0;
    rx_tail    = 0;
    uart_ready = 1;
    memset(rx_buffer, 0, sizeof(rx_buffer));
}

void uart_transmit(uint8_t byte)
{
    if (!uart_ready) return;
    putchar((int)byte);
}

uint8_t uart_receive(void)
{
    if (rx_head == rx_tail) return 0;
    uint8_t data = rx_buffer[rx_tail];
    rx_tail = (rx_tail + 1) % sizeof(rx_buffer);
    return data;
}

void uart_print(const char *str)
{
    if (!str) return;
    while (*str) {
        uart_transmit((uint8_t)(*str++));
    }
}

uint8_t uart_data_available(void)
{
    return (rx_head != rx_tail) ? 1 : 0;
}
