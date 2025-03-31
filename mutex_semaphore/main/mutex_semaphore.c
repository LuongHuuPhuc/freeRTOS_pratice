/**
 * @note Task Mutex co co che uu tien 
 * Task nao "take" thi task day phai "give"
 */
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_timer.h"
#include "freertos/semphr.h"

SemaphoreHandle_t xSemaphore = NULL;

void vTask1(){
  xSemaphoreTake(xSemaphore, portMAX_DELAY); //Lay tai nguyen 
  for(size_t i = 0; i < 100; i++){
    printf("Task1: %d: %lld\n", i, esp_timer_get_time() / 1000);
    vTaskDelay(1000 / portTICK_PERIOD_MS); //Delay 1000ms
  }

  xSemaphoreGive(xSemaphore); //Giai phong mutex 
  vTaskDelete(NULL); //Xoa task de giai phong bo nho
}

void app_main(){
  xSemaphore = xSemaphoreCreateMutex();
  printf("\nTimer output in milliseconds program initation: %lld\n\n", esp_timer_get_time() / 1000); //In thoi gian bat dau
  
  //Song song 2 task o 2 loi
  xTaskCreatePinnedToCore(vTask1, "Task1", 2048, NULL, 2, NULL, 0); //Task1 xu ly o core 0
  vTaskDelay(1000 / portTICK_PERIOD_MS); //Doi 1s 

  xSemaphoreTake(xSemaphore, portMAX_DELAY); //Lay mutex 
  printf("\nProgram completion: %lld\n", esp_timer_get_time() / 1000);
  xSemaphoreGive(xSemaphore); //Thuc thi ham xong thi tra lai luon
}