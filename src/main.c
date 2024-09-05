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
#include <zephyr/logging/log.h>
LOG_MODULE_REGISTER(freebot, LOG_LEVEL_DBG);

#include "freebot.h"
#define FB_BENCH_SPEED (20 * 100 / 80)

static fb_motor_speed_t rpm;
static int v_cap;

int main(void)
{
    // Initialize FreeBot
    fb_init();

    // Turn both leds off
    fb_clear_led(D15);
    fb_clear_led(D16);

    // Disable bot-to-bot charging port
    fb_b2b_disable();

    // Select Vcap for measurements
    fb_v_measure_select(V_CAP);

    // Start to drive forward with benchmarking speed
    fb_straight_forw(FB_BENCH_SPEED);
    
    // Wait forever (until energy is gone)
    for(;;) {
        v_cap = fb_v_measure();
        fb_get_motor_speed(&rpm);
        LOG_DBG("[%d, %d, %d, %d, %d]", v_cap, rpm.front_left, rpm.front_right, rpm.back_left, rpm.back_right);
        k_sleep(K_MSEC(1000));
    }
    // k_sleep(K_FOREVER);
    return 0;
}
