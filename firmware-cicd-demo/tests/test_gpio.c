#include "../lib/Unity/unity.h"
#include "../src/drivers/gpio.h"

void setUp(void)
{
    gpio_init();
}

void tearDown(void) {}

void test_gpio_init_sets_defaults(void)
{
    /* After init, all pins should read LOW */
    TEST_ASSERT_EQUAL_UINT8(0, gpio_read(0));
    TEST_ASSERT_EQUAL_UINT8(0, gpio_read(7));
    TEST_ASSERT_EQUAL_UINT8(0, gpio_read(15));
}

void test_gpio_write_high(void)
{
    gpio_write(5, 1);
    TEST_ASSERT_EQUAL_UINT8(1, gpio_read(5));
}

void test_gpio_write_low(void)
{
    gpio_write(5, 1);
    gpio_write(5, 0);
    TEST_ASSERT_EQUAL_UINT8(0, gpio_read(5));
}

void test_gpio_toggle(void)
{
    gpio_write(3, 0);
    gpio_toggle(3);
    TEST_ASSERT_EQUAL_UINT8(1, gpio_read(3));
    gpio_toggle(3);
    TEST_ASSERT_EQUAL_UINT8(0, gpio_read(3));
}

void test_gpio_led_set_on(void)
{
    /* Should not crash; LED is simulated */
    gpio_led_set(1);
    /* No assert needed — testing it runs without fault */
    TEST_PASS();
}

void test_gpio_led_set_off(void)
{
    gpio_led_set(1);
    gpio_led_set(0);
    TEST_PASS();
}

void test_gpio_invalid_pin_does_not_crash(void)
{
    /* Pin > 15 is invalid — driver should ignore it gracefully */
    gpio_write(255, 1);
    gpio_toggle(200);
    TEST_ASSERT_EQUAL_UINT8(0, gpio_read(200));
}

int main(void)
{
    UNITY_BEGIN();

    RUN_TEST(test_gpio_init_sets_defaults);
    RUN_TEST(test_gpio_write_high);
    RUN_TEST(test_gpio_write_low);
    RUN_TEST(test_gpio_toggle);
    RUN_TEST(test_gpio_led_set_on);
    RUN_TEST(test_gpio_led_set_off);
    RUN_TEST(test_gpio_invalid_pin_does_not_crash);

    return UNITY_END();
}
