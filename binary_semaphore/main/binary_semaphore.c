/**
 * @note Chuong trinh tao 1 task chay doc lap va su dung semaphore de dong bo hoa
 */
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_timer.h"
#include "freertos/semphr.h"

SemaphoreHandle_t xSemaphore = NULL;

void vTask1(){
  for(size_t i = 0; i < 100; i++){
    printf("Task1: %d: %lld\n", i, esp_timer_get_time() / 1000);
    vTaskDelay(1000 / portTICK_PERIOD_MS); //Delay 1000ms
  }
  //Sau khi chay xong vong lap thi giai phong semaphore de bao cho app_main la task da ket thuc
  xSemaphoreGive(xSemaphore);
  vTaskDelete(NULL); //Xoa task de giai phong bo nho
}

void app_main(){
  xSemaphore = xSemaphoreCreateBinary(); //Tao semaphore kieu binary (1 = co san, 0 = ko co san)
  printf("\nTimer output in milliseconds program initation: %lld\n\n", esp_timer_get_time() / 1000); //In thoi gian bat dau
  
  //Song song 2 task o 2 loi
  xTaskCreatePinnedToCore(vTask1, "Task1", 2048, NULL, 2, NULL, 0); //Task1 xu ly o core 0

  //Doi task 1 hoan thanh de nhan lai semaphore thi in ra man hinh thoi gian
  if(xSemaphoreTake(xSemaphore, portMAX_DELAY)){
    printf("\nProgram completion: %lld\n", esp_timer_get_time() / 1000);
  }
}