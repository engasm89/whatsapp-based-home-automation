/*
 * Course: Whatsapp Based Home Automation
 * Platform: Embedded Systems
 * Author: Ashraf S A AlMadhoun
 * 
 * Description:
 * This code demonstrates the core concepts taught in the "Whatsapp Based Home Automation" course.
 * It provides a practical implementation that students can study, modify, and
 * use as a foundation for their own projects.
 * 
 * Learning Objectives:
 * - Understand the fundamental principles covered in this course
 * - Practice implementing the concepts with real code
 * - Build a working example that can be extended
 * 
 * Hardware Requirements:
 * - Development board (Embedded Systems)
 * - Components as specified in CIRCUIT.md
 * - USB cable for programming
 * 
 * Pin Connections:
 * Refer to CIRCUIT.md for detailed wiring diagrams and pin assignments.
 * 
 * Usage:
 * 1. Review the code structure and comments
 * 2. Connect hardware according to CIRCUIT.md
 * 3. Upload code to your development board
 * 4. Monitor serial output for debugging
 * 
 * Course Link: https://www.udemy.com/course/whatsapp-based-home-automation/
 * Repository: https://github.com/engasm89/whatsapp-based-home-automation
 * 
 * Copyright (c) 2025 Ashraf S A AlMadhoun
 * Licensed under MIT License
 */

#include "freertos/FreeRTOS.h" // FreeRTOS
#include "freertos/task.h" // Tasks
#include "driver/gpio.h" // GPIO
#include "esp_log.h" // Logging
#include "esp_http_client.h" // HTTP

#define RELAY GPIO_NUM_25 // Relay GPIO
static const char *TAG = "WHATSAPP"; // Tag
static int backoff_ms = 2000; // Retry backoff

static void webhook_task(void *arg) { // Poll webhook for commands
  esp_http_client_config_t cfg = { .url = "https://example.com/whatsapp/webhook" }; // Placeholder URL
  esp_http_client_handle_t client = esp_http_client_init(&cfg); // Init client
  while (true) { // Loop
    if (esp_http_client_perform(client) == ESP_OK) { // Perform GET
      int code = esp_http_client_get_status_code(client); // Status
      int relay_cmd = (code == 200); // Simulated command
      gpio_set_level(RELAY, relay_cmd); // Drive relay
      ESP_LOGI(TAG, "Webhook status=%d relay=%d", code, relay_cmd); // Log
      backoff_ms = 2000; // Reset backoff
      vTaskDelay(pdMS_TO_TICKS(3000)); // Normal poll interval
    } else { // Error
      ESP_LOGW(TAG, "Webhook failed, backoff=%dms", backoff_ms); // Warn
      vTaskDelay(pdMS_TO_TICKS(backoff_ms)); // Backoff delay
      backoff_ms = backoff_ms < 60000 ? backoff_ms * 2 : 60000; // Exponential backoff
    } // End if
  } // End loop
} // End task

void app_main(void) { // Program entry point
  gpio_set_direction(RELAY, GPIO_MODE_OUTPUT); // Relay out
  gpio_set_level(RELAY, 0); // Ensure off
  ESP_LOGI(TAG, "WhatsApp automation ready"); // Log
  xTaskCreate(webhook_task, "whatsapp", 4096, NULL, 5, NULL); // Create task
} // End of main function

