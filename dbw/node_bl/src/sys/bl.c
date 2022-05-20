#include "sys/bl.h"

#include <driver/timer.h>
#include <esp_err.h>
#include <esp_ota_ops.h>
#include <esp_system.h>

#include "io/can.h"

// ######        DEFINES        ###### //

// ######      PROTOTYPES       ###### //

// ######     PRIVATE DATA      ###### //

static uint64_t bl_init_timer_val;

static const esp_partition_t *ota_partition;

// ######          CAN          ###### //

// ######   PRIVATE FUNCTIONS   ###### //

// ######   PUBLIC FUNCTIONS    ###### //

esp_err_t bl_init(void)
{
    esp_err_t err;

    ota_partition = esp_partition_find_first(
        ESP_PARTITION_TYPE_APP,
        ESP_PARTITION_SUBTYPE_APP_OTA_0,
        NULL
    );
    if (!ota_partition) return ESP_FAIL;

    err = timer_get_counter_value(CAN_TIMER_GROUP, CAN_TIMER, &bl_init_timer_val);
    if (err != ESP_OK) return err;

    return ESP_OK;
}

void bl_restart(void)
{
    // if our OTA partition is invalid changing the boot partition will
    // fail and we'll boot into the bootloader anyways
    esp_ota_set_boot_partition(ota_partition);
    esp_restart();
}
