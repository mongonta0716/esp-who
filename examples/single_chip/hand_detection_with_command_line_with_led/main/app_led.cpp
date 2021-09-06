/*
  * ESPRESSIF MIT License
  *
  * Copyright (c) 2020 <ESPRESSIF SYSTEMS (SHANGHAI) PTE LTD>
  *
  * Permission is hereby granted for use on ESPRESSIF SYSTEMS products only, in which case,
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
  *
  */
#include <string.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "esp_log.h"
#include "app_led.h"
#define I2S_DEVICE 0
#include "FastLED.h"

static const char *TAG = "led";

static SemaphoreHandle_t query_lock = NULL;
static bool hand_detected = false;
static bool last_hand_detected = hand_detected;
//extern int hand_detected;

#define NUM_LEDS 15
#define LED_TYPE WS2811
#define LED_PIN 4
CRGB leds[NUM_LEDS];
static bool change_status = false;

static const uint32_t wait_led = 0;
static const uint32_t wait_for_led_on  = 10000UL; // LEDが点灯している間の待ち時間
static const uint32_t wait_for_led_off = 1000UL;  // LEDが消灯している間の待ち時間
static uint32_t wait_for_change = wait_for_led_off;

static uint32_t last_millis = 0;

static void led_task(void* args) {
    BaseType_t xStatus; 
//    for (;;) {
        if (hand_detected == true and change_status == true) {
            ESP_LOGI(TAG, "****************************************:LED on");
            for (int i=0; i<NUM_LEDS; i++) {
                leds[i] = CRGB::AntiqueWhite;
                xStatus = xSemaphoreTake(query_lock, portMAX_DELAY);
                if (xStatus == pdTRUE) {
                    FastLED.show();
                }
                vTaskDelay(wait_led/portTICK_PERIOD_MS);
                xSemaphoreGive(query_lock);
            }
            change_status = false;
            wait_for_change = wait_for_led_on;
        } else if (hand_detected == false and change_status == true) { 
            ESP_LOGI(TAG, "****************************************:LED off");
            
            for (int i=0; i<NUM_LEDS; i++) {
                leds[i] = CRGB::Black;
                xStatus = xSemaphoreTake(query_lock, portMAX_DELAY);
                if (xStatus == pdTRUE) {
                    FastLED.clearData();
                    FastLED.show();
                }
                vTaskDelay(wait_led/portTICK_PERIOD_MS);
                xSemaphoreGive(query_lock);
            }
            change_status = false;
            wait_for_change = wait_for_led_off;
        } else {
            leds[0] = CRGB::HTMLColorCode::DarkBlue;
            xStatus = xSemaphoreTake(query_lock, portMAX_DELAY);
            if (xStatus == pdTRUE) {
                FastLED.show();
            }
            xSemaphoreGive(query_lock);
   
            leds[0] = CRGB::HTMLColorCode::White;
            FastLED.show();
            vTaskDelay(100/portTICK_PERIOD_MS);
            leds[0] = CRGB::HTMLColorCode::Black;
            FastLED.show();
            vTaskDelay(100/portTICK_PERIOD_MS);
        }
//        vTaskDelay(wait_for_change/portTICK_PERIOD_MS);
//    }
    vTaskDelete(NULL);
}

/*
static unsigned long millis() { 
    return (unsigned long)(esp_timer_get_time() / 1000ULL);
}
*/

/*
*  Public Functions
*/

extern "C" {


void set_hand_detect(bool status) {
    if (last_hand_detected != status) {
        if ((millis() - last_millis) > wait_for_change) {
            ESP_LOGI(TAG, "hand_detected change:%s", (status) ? "true" : "false");
            last_hand_detected = hand_detected;
            xTaskCreatePinnedToCore(led_task, "led_task", 2048, NULL, 5, NULL, 1);
            last_millis = millis();
            ESP_LOGI(TAG, "last_millis: %ld", (unsigned long)last_millis);
        } 
        change_status = true;
        hand_detected = status;
    }
    if (status) {
        // 手が認識されていた場合、時間を伸ばす
        last_millis = millis();
    }
}

void app_led_main()
{	
    last_millis = millis();
    FastLED.addLeds<WS2811, LED_PIN>(leds, NUM_LEDS);

    for (int i=0; i<NUM_LEDS; i++) {
        leds[i] = CRGB::Blue;
        FastLED.show();
        ESP_LOGI(TAG, "led white");
        vTaskDelay(50/portTICK_PERIOD_MS);
        leds[i] = CRGB::Black;
        FastLED.show();
        ESP_LOGI(TAG, "led black");
        vTaskDelay(50/portTICK_PERIOD_MS);
    }

	query_lock = xSemaphoreCreateBinary();
	if (query_lock == NULL) {
        ESP_LOGE(TAG, "xSemaphoreCreateMutex() Failed");
        return;
	}
	xSemaphoreGive(query_lock);

}
}// extern "C"