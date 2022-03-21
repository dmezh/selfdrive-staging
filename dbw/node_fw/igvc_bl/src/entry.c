#include <driver/gpio.h>

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <driver/twai.h>
#include <string.h>

#include "esp_ota_ops.h"

#include "can.h"
#include "can_gen.h"

#define GPIO_LED1 GPIO_NUM_33
#define GPIO_LED2 GPIO_NUM_32

static uint8_t fw_image[1024 * 128];

// CAN

static struct CAN_dbwBootloader_Data_t can_Bootloader_Data;
static can_outgoing_t can_Bootloader_Data_cfg = {
    .id = CAN_DBWBOOTLOADER_DATA_FRAME_ID,
    .extd = CAN_DBWBOOTLOADER_DATA_IS_EXTENDED,
    .dlc = CAN_DBWBOOTLOADER_DATA_LENGTH,
    .pack = CAN_dbwBootloader_Data_pack,
};

static struct CAN_dbwUpdater_Meta_t can_Updater_Meta = {0};
static can_incoming_t can_Updater_Meta_cfg = {
    .id = CAN_DBWUPDATER_META_FRAME_ID,
    .out = &can_Updater_Meta,
    .unpack = CAN_dbwUpdater_Meta_unpack,
};

static struct CAN_dbwUpdater_Data_t can_Updater_Data = {0};
static can_incoming_t can_Updater_Data_cfg = {
    .id = CAN_DBWUPDATER_DATA_FRAME_ID,
    .out = &can_Updater_Data,
    .unpack = CAN_dbwUpdater_Data_unpack,
};

#define SEND_BL_DATA() can_send_iface(&can_Bootloader_Data_cfg, &can_Bootloader_Data)

void app_main()
{
    can_init();

    gpio_pad_select_gpio(GPIO_LED1);
    gpio_set_direction(GPIO_LED1, GPIO_MODE_OUTPUT);
    gpio_set_level(GPIO_LED1, 0);

    gpio_pad_select_gpio(GPIO_LED2);
    gpio_set_direction(GPIO_LED2, GPIO_MODE_OUTPUT);
    gpio_set_level(GPIO_LED2, 0);

    can_register_incoming_msg(can_Updater_Meta_cfg);
    can_register_incoming_msg(can_Updater_Data_cfg);

    bool state = 0;

    TaskHandle_t can_get_handle = NULL;
    xTaskCreatePinnedToCore(can_get, "can_get", 10000, 0, 0, &can_get_handle, 1);

    for (;;) {
        can_Bootloader_Data.OTAError = CAN_dbwBootloader_Data_OTAError_OTA_NO_ERROR_YET_CHOICE;
        gpio_set_level(GPIO_LED1, 1);
        vTaskDelay(200 / portTICK_PERIOD_MS);

        state = !state;

        const esp_partition_t* running = esp_ota_get_running_partition();
        const esp_partition_t* boot = esp_ota_get_boot_partition();
        //const esp_partition_t* update = esp_ota_get_next_update_partition(NULL);
        const esp_partition_t* update = esp_partition_find_first(ESP_PARTITION_TYPE_APP, ESP_PARTITION_SUBTYPE_APP_OTA_0, NULL);

        can_Bootloader_Data.RunningPartitionType = running ? running->subtype : -1;
        can_Bootloader_Data.BootPartitionType = boot ? boot->subtype : -1;
        can_Bootloader_Data.UpdatePartitionType = update ? update->subtype : -1;
        SEND_BL_DATA();

        bool led2_state = 0;

        // wait for update begin
        while (!can_Updater_Meta.Begin) {
            vTaskDelay(200 / portTICK_RATE_MS);
            led2_state = !led2_state;
            gpio_set_level(GPIO_LED2, led2_state);
        }
        
        gpio_set_level(GPIO_LED1, 1);
        gpio_set_level(GPIO_LED2, 1);

        can_Bootloader_Data.Ready = 1;
        SEND_BL_DATA();

        // write the firmware image data
        int last_position = -1;
        while (!can_Updater_Meta.FinalSize) {
            const int position = can_Updater_Data.Position;

            if (position != last_position) {
                last_position = position;

                memcpy(fw_image + (position * 5), &can_Updater_Data.Data, 5);
            }
        }

        // write the update to flash
        gpio_set_level(GPIO_LED1, 0);
        gpio_set_level(GPIO_LED2, 1);

        esp_ota_handle_t update_handle = 0;

        esp_err_t err = esp_ota_begin(update, OTA_WITH_SEQUENTIAL_WRITES, &update_handle);
        if (err != ESP_OK) {
            esp_ota_abort(update_handle);
            can_Bootloader_Data.OTAError = CAN_dbwBootloader_Data_OTAError_OTA_BEGIN_ERR_CHOICE;
            can_Bootloader_Data.EspErrCode = err;
            SEND_BL_DATA();
            continue;
        }

        err = esp_ota_write(update_handle, fw_image, can_Updater_Meta.FinalSize);
        if (err != ESP_OK) {
            can_Bootloader_Data.OTAError = CAN_dbwBootloader_Data_OTAError_OTA_WRITE_ERR_CHOICE;
            can_Bootloader_Data.EspErrCode = err;
            SEND_BL_DATA();
            continue;
        }

        err = esp_ota_end(update_handle);
        if (err != ESP_OK) {
            can_Bootloader_Data.OTAError = CAN_dbwBootloader_Data_OTAError_OTA_END_ERR_CHOICE;
            can_Bootloader_Data.EspErrCode = err;
            SEND_BL_DATA();
            continue;
        }

        err = esp_ota_set_boot_partition(update);
        if (err != ESP_OK) {
            can_Bootloader_Data.OTAError = CAN_dbwBootloader_Data_OTAError_OTA_SET_BOOT_PARTITION_ERR_CHOICE;
            can_Bootloader_Data.EspErrCode = err;
            SEND_BL_DATA();
            continue;
        }

        esp_restart();
    }
}
