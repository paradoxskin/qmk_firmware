#include "config.h"
#include "aw20216s.h"
#include <stdio.h>

aw20216s_led_t PROGMEM g_aw20216s_leds[AW20216S_LED_COUNT] = {0};

void test_r(void)
{
    static uint8_t now = 0;
    g_aw20216s_leds[0].r = now;
    aw20216s_set_color_all(0, 100, 0);
    aw20216s_flush();
    printf("0x%x 0x%x 0x%x\n", g_aw20216s_leds[0].r,
        g_aw20216s_leds[0].g,
        g_aw20216s_leds[0].b);
    now++;
    now %= 0xD8;
}

void test_g(void)
{
    static uint8_t now = 0;
    g_aw20216s_leds[0].g = now;
    aw20216s_set_color_all(0, 0, 100);
    aw20216s_flush();
    printf("0x%x 0x%x 0x%x\n", g_aw20216s_leds[0].r,
        g_aw20216s_leds[0].g,
        g_aw20216s_leds[0].b);
    now++;
    now %= 0xD8;
}

void test_b(void)
{
    static uint8_t now = 0;
    g_aw20216s_leds[0].b = now;
    aw20216s_set_color_all(100, 0, 0);
    aw20216s_flush();
    printf("0x%x 0x%x 0x%x\n", g_aw20216s_leds[0].r,
        g_aw20216s_leds[0].g,
        g_aw20216s_leds[0].b);
    now++;
    now %= 0xD8;
}
