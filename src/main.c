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

#include "zephyr/drivers/spi.h"
#include <sys/_stdint.h>
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(freebot, LOG_LEVEL_DBG);

#include "freebot.h"
#define FB_DEMO_DELAY K_MSEC(500)
#define FB_DEMO_SPEED 75

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
    fb_straight_forw(FB_DEMO_SPEED);
    LOG_DBG("Robot going forward");
    k_sleep(FB_DEMO_DELAY);

    fb_straight_back(FB_DEMO_SPEED);
    LOG_DBG("Robot going backward");
    k_sleep(FB_DEMO_DELAY);

    fb_side_right(FB_DEMO_SPEED);
    LOG_DBG("Robot going right");
    k_sleep(FB_DEMO_DELAY);

    fb_side_left(FB_DEMO_SPEED);
    LOG_DBG("Robot going left");
    k_sleep(FB_DEMO_DELAY);

    fb_side_d45(FB_DEMO_SPEED);
    LOG_DBG("Robot going 45°");
    k_sleep(FB_DEMO_DELAY);

    fb_side_d135(FB_DEMO_SPEED);
    LOG_DBG("Robot going 135°");
    k_sleep(FB_DEMO_DELAY);

    fb_side_d225(FB_DEMO_SPEED);
    LOG_DBG("Robot going 225°");
    k_sleep(FB_DEMO_DELAY);

    fb_side_d315(FB_DEMO_SPEED);
    LOG_DBG("Robot going 315°");
    k_sleep(FB_DEMO_DELAY);

    fb_rotate_cw(FB_DEMO_SPEED);
    LOG_DBG("Robot rotating clockwise");
    k_sleep(FB_DEMO_DELAY);

    fb_rotate_ccw(FB_DEMO_SPEED);
    LOG_DBG("Robot rotating counterclockwise");
    k_sleep(FB_DEMO_DELAY);

    fb_stop();
    LOG_DBG("Robot stopped");
}

void motor_measure_demo(void)
{
    fb_get_motor_angle(&motor_angles);
    LOG_DBG("{%d°, %d°, %d°, %d°}", motor_angles.front_left, motor_angles.front_right,
            motor_angles.back_left, motor_angles.back_right);

    fb_rotate_cw(FB_DEMO_SPEED);
    k_sleep(K_MSEC(200));
    fb_get_motor_speed(&motor_speeds);
    LOG_DBG("{%d rpm, %d rpm, %d rpm, %d rpm}", motor_speeds.front_left, motor_speeds.front_right,
            motor_speeds.back_left, motor_speeds.back_right);

    fb_stop();
    for (int i = 0; i < 4; i++)
    {
        k_sleep(K_MSEC(100));
        fb_get_motor_speed(&motor_speeds);
        LOG_DBG("{%d rpm, %d rpm, %d rpm, %d rpm}", motor_speeds.front_left,
                motor_speeds.front_right, motor_speeds.back_left, motor_speeds.back_right);
    }

    fb_get_motor_angle(&motor_angles);
    LOG_DBG("{%d°, %d°, %d°, %d°}", motor_angles.front_left, motor_angles.front_right,
            motor_angles.back_left, motor_angles.back_right);
}

void motor_speed_demo(void)
{
    const int STEPS = 10;
    for (int i = 0; i <= STEPS; i++)
    {
        fb_rotate_cw(100 * i / STEPS);
        LOG_DBG("Target speed: %d", 100 * i / STEPS);
        for (int j = 0; j < 10; j++)
        {
            k_sleep(K_MSEC(100));
            fb_get_motor_speed(&motor_speeds);
            LOG_DBG("{%d rpm, %d rpm, %d rpm, %d rpm}", motor_speeds.front_left,
                    motor_speeds.front_right, motor_speeds.back_left, motor_speeds.back_right);
        }
    }
    fb_stop();
}

int main(void)
{
    LOG_DBG("Starting Robot");
    fb_init();

    for (;;)
    {
        // Enable bot-to-bot charging port when waiting
        fb_b2b_enable();

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

            // DEBUG: try SPI interface
            uint8_t tx_buf[] = "Hello from SPI\n";
            struct spi_buf tx_spi_buf = {.buf = (void *)&tx_buf, .len = sizeof(tx_buf)};
            struct spi_buf_set tx_spi_buf_set = {.buffers = &tx_spi_buf, .count = 1};
            fb_spi_tranceive(&tx_spi_buf_set, NULL);
        }

        // Turn both leds off during operation
        fb_clear_led(D15);
        fb_clear_led(D16);

        // Disable bot-to-bot charging port during operation
        fb_b2b_disable();

        pwr_measure_demo();
        motor_measure_demo();
        motor_drive_demo();
        motor_speed_demo();
    }
    return 0;
}
