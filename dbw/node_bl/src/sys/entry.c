#include <esp_err.h>
#include <stdbool.h>

#include "io/can.h"
#include "sys/bl.h"
#include "sys/blink.h"
#include "sys/timing.h"

void app_main()
{
    esp_err_t err;

    err = timing_init();
    if (err != ESP_OK) bl_restart();

    err = blink_init();
    if (err != ESP_OK) bl_restart();

    err = can_init();
    if (err != ESP_OK) bl_restart();

    err = bl_init();
    if (err != ESP_OK) bl_restart();

    while (true) {
        err = blink_pulse();
        if (err != ESP_OK) bl_restart();

        err = can_poll();
        if ((err != ESP_OK) && (err != ESP_ERR_TIMEOUT)) bl_restart();

        err = bl_magic_wait();
        if (err == ESP_OK) break;
    }
}
