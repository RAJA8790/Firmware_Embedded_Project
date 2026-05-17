#include "../lib/Unity/unity.h"
#include "../src/drivers/uart.h"

void setUp(void)
{
    uart_init(115200);
}

void tearDown(void) {}

void test_uart_init_with_standard_baud(void)
{
    /* uart_init must not crash with standard baud rates */
    uart_init(9600);
    uart_init(115200);
    uart_init(921600);
    TEST_PASS();
}

void test_uart_no_data_available_after_init(void)
{
    TEST_ASSERT_EQUAL_UINT8(0, uart_data_available());
}

void test_uart_transmit_does_not_crash(void)
{
    /* Transmitting over simulated UART should not crash */
    uart_transmit('H');
    uart_transmit('i');
    uart_transmit('\r');
    uart_transmit('\n');
    TEST_PASS();
}

void test_uart_print_does_not_crash(void)
{
    uart_print("Hello CI!\r\n");
    TEST_PASS();
}

void test_uart_print_null_does_not_crash(void)
{
    /* Passing NULL must be handled gracefully */
    uart_print(NULL);
    TEST_PASS();
}

void test_uart_receive_returns_zero_when_empty(void)
{
    /* Nothing in RX buffer → should return 0 */
    TEST_ASSERT_EQUAL_UINT8(0, uart_receive());
}

int main(void)
{
    UNITY_BEGIN();

    RUN_TEST(test_uart_init_with_standard_baud);
    RUN_TEST(test_uart_no_data_available_after_init);
    RUN_TEST(test_uart_transmit_does_not_crash);
    RUN_TEST(test_uart_print_does_not_crash);
    RUN_TEST(test_uart_print_null_does_not_crash);
    RUN_TEST(test_uart_receive_returns_zero_when_empty);

    return UNITY_END();
}
