#include "can.h"

#include <esp_ota_ops.h>
#include <driver/twai.h>
#include <stdio.h>

#include "base/base.h"
#include "common.h"
#include "sys/task_glue.h"
#include "sys/watchdog.h"

// ######        DEFINES        ###### //

#define CAN_TX_GPIO 19
#define CAN_RX_GPIO 18

// ######      PROTOTYPES       ###### //

static void can_init();
static void can_1Hz_ping();

// ######     PRIVATE DATA      ###### //

static bool can_ready;

// ######    RATE FUNCTIONS     ###### //

struct rate_funcs can_rf = {
    .call_init = can_init,
    .call_1Hz = can_1Hz_ping,
};

static void can_init()
{
    twai_general_config_t g_config = TWAI_GENERAL_CONFIG_DEFAULT(CAN_TX_GPIO, CAN_RX_GPIO, TWAI_MODE_NORMAL);
    twai_timing_config_t t_config = TWAI_TIMING_CONFIG_500KBITS();
    twai_filter_config_t f_config = TWAI_FILTER_CONFIG_ACCEPT_ALL();

    if (twai_driver_install(&g_config, &t_config, &f_config) == ESP_OK) {
        can_ready = 1;
    } else {
        can_ready = 0;
    }

    if (twai_start() == ESP_OK) {
        can_ready &= 1;
    } else {
        can_ready &= 0;
    }
}

static void can_1Hz_ping()
{
    static uint8_t count = 0;

    if (!can_ready) {
        base_set_state_lost_can();
        return;
    }

    twai_message_t message;
    message.extd = 0;

    message.identifier = 0x1;

    message.data_length_code = 2;

    message.data[0] = 0;
    message.data[1] = count++;
    message.data[2] = 0;
    message.data[3] = 0;

    can_send_msg(&message);
}

// ######   PRIVATE FUNCTIONS   ###### //

// ######   PUBLIC FUNCTIONS    ###### //

void can_send_msg(const twai_message_t *message) {
    esp_err_t r = twai_transmit(message, 0);

    if (r == ESP_OK) {
        base_set_state_good();
    } else {
        base_set_state_lost_can();
        // attempt recovery?
    }

    if (count == 0x50) {
        const esp_partition_t* factory = esp_partition_find_first(
            ESP_PARTITION_TYPE_APP,
            ESP_PARTITION_SUBTYPE_APP_FACTORY,
            NULL
        );
        if (!factory) {
            count = 0x99;
        } else {
            set_up_rtc_watchdog_fwupdate(); // give us some time
            esp_err_t err = esp_ota_set_boot_partition(factory);
            if (err == ESP_OK)
                esp_restart();

            count = err;
        }
    }

    if (count == 0x51) {
        esp_restart();
    }

}
