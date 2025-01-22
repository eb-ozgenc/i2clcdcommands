#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"

#include "i2clcd.h"

void app_main(void){   
    i2cmasterinit();
    i2clcdinit();
    i2clcdwritestring("Hello World");
    i2clcdline2();
    i2clcdsendnum(128);

    ESP_LOGI("main", "initialized");

    while (1) {
        vTaskDelay(pdMS_TO_TICKS(100));
    }
}