#include <driver/gpio.h>

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <driver/twai.h>

#include "esp_ota_ops.h"

#include "can.h"

#define GPIO_LED1 GPIO_NUM_33

void can_send(int id, int val)
{
    twai_message_t message;
    message.extd = 0;
    message.rtr = 0;

    message.identifier = id;

    message.data_length_code = 1;

    message.data[0] = val;
    message.data[1] = 0;
    message.data[2] = 0;
    message.data[3] = 0;

    esp_err_t r = twai_transmit(&message, 0);
}

extern const uint8_t firmware_bin_start[] asm("_binary_fullfw_bin_start");
extern const uint8_t firmware_bin_end[] asm("_binary_fullfw_bin_end");

void app_main()
{
    can_init();

    gpio_pad_select_gpio(GPIO_LED1);
    gpio_set_direction(GPIO_LED1, GPIO_MODE_OUTPUT);

    bool state = 0;

    for (;;) {
        gpio_set_level(GPIO_LED1, state);
        vTaskDelay(200 / portTICK_PERIOD_MS);
        state = !state;
        can_ping();

        const esp_partition_t* running = esp_ota_get_running_partition();
        const esp_partition_t* boot = esp_ota_get_boot_partition();
        //const esp_partition_t* update = esp_ota_get_next_update_partition(NULL);
        const esp_partition_t* update = esp_partition_find_first(ESP_PARTITION_TYPE_APP, ESP_PARTITION_SUBTYPE_APP_OTA_0, NULL);

        if (running)
            can_send(0x10, running->subtype);
        if (boot)
            can_send(0x11, boot->subtype);
        if (update)
            can_send(0x12, update->subtype);

        // can_send(0x13, esp_ota_get_app_partition_count());

        esp_ota_handle_t update_handle = 0;
        esp_err_t err = esp_ota_begin(update, OTA_WITH_SEQUENTIAL_WRITES, &update_handle);

        can_send(0x14, err);
        if (err != ESP_OK) {
            esp_ota_abort(update_handle);
            continue;
        }

        err = esp_ota_write(update_handle, firmware_bin_start, firmware_bin_end - firmware_bin_start);

        can_send(0x15, err);
        if (err != ESP_OK) {
            esp_ota_abort(update_handle);
            continue;
        }

        err = esp_ota_end(update_handle);
        can_send(0x16, err);
        if (err != ESP_OK) {
            continue;
        }

        err = esp_ota_set_boot_partition(update);
        can_send(0x17, err);
        if (err != ESP_OK) {
            continue;
        }

        esp_restart();
    }
}
