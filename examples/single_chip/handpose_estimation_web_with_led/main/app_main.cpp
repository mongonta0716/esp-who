/* ESPRESSIF MIT License
 * 
 * Copyright (c) 2018 <ESPRESSIF SYSTEMS (SHANGHAI) PTE LTD>
 * 
 * Permission is hereby granted for use on all ESPRESSIF SYSTEMS products, in which case,
 * it is free of charge, to any person obtaining a copy of this software and associated
 * documentation files (the "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the Software is furnished
 * to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all copies or
 * substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */
#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include <esp_log.h>
#include "app_camera.h"
#include "app_wifi.h"
#include "app_httpd.h"
#include "app_mdns.h"
#include "app_led.h"
#include "timer_cam_config.h"
#include "battery.h"

const static char* TAG = "app_main";


extern "C" {


void app_main()
{
//    bat_init();
    ESP_LOGI(TAG, "app_main start");
    app_wifi_main();
    app_camera_main();
    app_httpd_main();
    app_mdns_main();
    app_led_main();
   
/*
    for (;;) {
        if (hand_detected == 1) {
            ESP_LOGI(TAG, "Blue");
            for (int i=0; i<NUM_LEDS; i++) {
                leds[i] = CRGB::White;
                FastLED.show();
                //vTaskDelay(100/portTICK_PERIOD_MS);
            }
        } else {
            ESP_LOGI(TAG, "Red");
            for (int i=0; i<NUM_LEDS; i++) {
                leds[i] = CRGB::Black;
                FastLED.show();
                //vTaskDelay(100/portTICK_PERIOD_MS);
            }
        }
        ESP_LOGI(TAG, "hand_detected:%d", hand_detected);
        vTaskDelay(1000/portTICK_PERIOD_MS);
    }
*/
}
}