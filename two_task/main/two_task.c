#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/timers.h"
#include "esp_timer.h"

/**
 * @param portTICK_PERIOD_MS la bien chuyen doi ms sang tick
 * \if CONFIG_FREERTOS_HZ = 1000 thi portTICK_PERIOD_MS = 1
 * \if CONFIG_FREERTOS_HZ = 100 thi portTICK_PERIOD_MS = 10
 */
#define DELAY_TIME (1000 / portTICK_PERIOD_MS) //Delay 1s

void vTask1(){
  for(size_t i = 0; i < 50; i++){
    printf("Task1: %d: %lld\n", i, esp_timer_get_time() / 1000);
    vTaskDelay(DELAY_TIME);
  }
  vTaskDelete(NULL);
}

void vTask2(){
  for(size_t i = 0; i < 50; i++){
    printf("Task2: %d: %lld\n", i, esp_timer_get_time() / 1000);
    vTaskDelay(DELAY_TIME);
  }
  vTaskDelete(NULL);
}

void app_main(){
  printf("\nTimer output in milliseconds program initiation: %lld\n\n", esp_timer_get_time() / 1000);

  xTaskCreate(vTask1, "Task1", 2048, NULL, 1, NULL);
  xTaskCreate(vTask2, "Task2", 2048, NULL, 1, NULL);
  
  printf("\nProgram completion: %lld\n", esp_timer_get_time() / 1000);
}