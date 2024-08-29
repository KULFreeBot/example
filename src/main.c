/**
 * @file main.c
 * @author Lowie Deferme <lowie.deferme@kuleuven.be>
 * @brief FreeBot zephyr example
 * @version 0.1
 * @date 2024-03-26
 *
 * @copyright Copyright (c) 2024
 *
 */

#include <zephyr/kernel.h>

#include "freebot.h"
#define FB_BENCH_SPEED 25

int main(void)
{
    // Initialize FreeBot
    fb_init();

    // Turn both leds off
    fb_clear_led(D15);
    fb_clear_led(D16);

    // Disable bot-to-bot charging port
    fb_b2b_disable();

    // Start to drive forward with benchmarking speed
    fb_straight_forw(FB_BENCH_SPEED);
    
    // Wait forever (until energy is gone)
    k_sleep(K_FOREVER);
    return 0;
}
