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
#define FB_DEMO_DELAY K_MSEC(500)

static fb_motor_angle_t motor_angles;
static fb_motor_speed_t motor_speeds;

void pwr_measure_demo(void)
{
    fb_v_measure_select(V_CAP);
    k_sleep(FB_DEMO_DELAY);
    int v_cap = fb_v_measure();
    LOG_DBG("Vcap = %dmV", v_cap);
}

void motor_drive_demo(void)
{
    fb_straight_forw();
    LOG_DBG("Robot going forward");
    k_sleep(FB_DEMO_DELAY);

    fb_straight_back();
    LOG_DBG("Robot going backward");
    k_sleep(FB_DEMO_DELAY);

    fb_side_right();
    LOG_DBG("Robot going right");
    k_sleep(FB_DEMO_DELAY);

    fb_side_left();
    LOG_DBG("Robot going left");
    k_sleep(FB_DEMO_DELAY);

    fb_side_d45();
    LOG_DBG("Robot going 45°");
    k_sleep(FB_DEMO_DELAY);

    fb_side_d135();
    LOG_DBG("Robot going 135°");
    k_sleep(FB_DEMO_DELAY);

    fb_side_d225();
    LOG_DBG("Robot going 225°");
    k_sleep(FB_DEMO_DELAY);

    fb_side_d315();
    LOG_DBG("Robot going 315°");
    k_sleep(FB_DEMO_DELAY);

    fb_rotate_cw();
    LOG_DBG("Robot rotating clockwise");
    k_sleep(FB_DEMO_DELAY);

    fb_rotate_ccw();
    LOG_DBG("Robot rotating counterclockwise");
    k_sleep(FB_DEMO_DELAY);

    fb_stop();
    LOG_DBG("Robot stopped");
}

void motor_measure_demo(void)
{
    fb_get_motor_angle(&motor_angles);
    LOG_DBG("{%d°, %d°, %d°, %d°}",
            motor_angles.front_left,
            motor_angles.front_right,
            motor_angles.back_left,
            motor_angles.back_right);

    fb_rotate_cw();
    k_sleep(K_MSEC(200));
    fb_get_motor_speed(&motor_speeds);
    LOG_DBG("{%d rpm, %d rpm, %d rpm, %d rpm}",
            motor_speeds.front_left,
            motor_speeds.front_right,
            motor_speeds.back_left,
            motor_speeds.back_right);

    fb_stop();
    for (int i = 0; i < 4; i++)
    {
        k_sleep(K_MSEC(100));
        fb_get_motor_speed(&motor_speeds);
        LOG_DBG("{%d rpm, %d rpm, %d rpm, %d rpm}",
                motor_speeds.front_left,
                motor_speeds.front_right,
                motor_speeds.back_left,
                motor_speeds.back_right);
    }

    fb_get_motor_angle(&motor_angles);
    LOG_DBG("{%d°, %d°, %d°, %d°}",
            motor_angles.front_left,
            motor_angles.front_right,
            motor_angles.back_left,
            motor_angles.back_right);
}

int main(void)
{
    LOG_DBG("Starting Robot");
    fb_init();

    for (;;)
    {
        // Flash both leds in out of sync to indicate ready
        fb_set_led(D15);
        fb_clear_led(D16);
        for (;;)
        {
            k_sleep(K_MSEC(100));
            fb_toggle_led(D15);
            fb_toggle_led(D16);

            if (fb_read_btn())
            {
                LOG_DBG("Button pressed");
                break;
            }
        }

        // Turn both leds off during operation
        fb_clear_led(D15);
        fb_clear_led(D16);

        pwr_measure_demo();
        motor_measure_demo();
        motor_drive_demo();
    }
    return 0;
}
