#include "freertos/FreeRTOS.h" // FreeRTOS
#include "freertos/task.h" // Tasks
#include "driver/gpio.h" // GPIO
#include "esp_log.h" // Logging
#include "esp_http_client.h" // HTTP

#define RELAY GPIO_NUM_25 // Relay GPIO
static const char *TAG = "WHATSAPP"; // Tag

void app_main(void) { // Entry
  gpio_set_direction(RELAY, GPIO_MODE_OUTPUT); // Relay out
  ESP_LOGI(TAG, "WhatsApp automation ready"); // Log
  // Placeholder: integrate webhook that sets relay state based on messages
  while (true) { // Loop
    gpio_set_level(RELAY, 1); // Relay on
    vTaskDelay(pdMS_TO_TICKS(1000)); // Delay
    gpio_set_level(RELAY, 0); // Relay off
    vTaskDelay(pdMS_TO_TICKS(1000)); // Delay
  } // End loop
} // End app_main

