# Làm quen với freeRTOS trong ESP32 # 
***
### *Tóm tắt các hàm delay trong freeRTOS* ###
1. `vTaskDelay()` <br>
Hàm này cho phép delay task theo đơn vị tick, giúp chuyển đổi tásk vào trạng thái "blocked" để CPU có thể thực hiện các task khác 
```c
#include "freertos/freeRTOS.h"
#include "freertos/task.h"

void TaskFunction(void *pvParameter){
  while(1){
    printf("Task is running...\n");
    vTaskDelay(1000 / portTICK_PERIOD_MS); //Delay 1000ms (1s)
  }
}
```

2. `vTaskDelayUntil()` (Delay thời gian tuyệt đối) <br>
Hàm này giúp đảm bảo khoảng thời gian giữa các lần chạy task không bị ảnh hưởng bởi độ trễ của các lệnh khác 
```c
void TaskFunction(void *pvParameter){
  TickType_t xLastWakeTime = xTaskGetTickCount(); //Lay thoi gian hien tai
  while(1){
    printf("Task runs at a fixed interval...\n");
    vTaskDelayUntil(&xLastWakeTime, 500 / portTICK_PERIOD_MS); //Delay chinh xac 500ms
  }
}
```
* Ham `vTasKDelayUntil()` giúp duy trì tần suất chạy của task mà không bị lệch do các lệnh khác
> Ví dụ: Nếu bạn cần chạy task mỗi 500ms, hàm này sẽ đảm bảo đúng khoảng thời gian đó.

3. `xTaskGetTickCount()` (Lấy thời gian hiện tại) <br>
Hàm này sẽ trả về số tick kể từ khi hệ thống bắt đầu chạy 
```c
TickType_t now  = xTaskGetTickCount();
printf("Current tick count: %d\n", now);
```

* Nếu `CONFIG_FREERTOS_HZ = 1000` thì mỗi tick tương ứng 1ms
* Nếu `CONFIG_FREERTOS_HZ = 100` thì mỗi tick tương ứng 10ms

4. `pdMS_TO_TICKS()` (Chuyển đổi từ milliseconds sang tick) <br>
Nếu bạn muốn viết code dễ đọc hơn, thay vì dùng `/ portTICK_PERIOD_MS` thì có thể dùng hàm trên
```c 
vTaskDelay(pdMS_TO_TICKS(1000)); //Delay 1000ms (1s)
```
*** 
### *Tổng quan các lệnh semaphore trong freeRTOS* ###
1. `xSemaphoreCreateMutex()` (Mutual Exclusion - Loại trừ lẫn nhau) <br>
*Kiểu:* Khóa tài nguyên <br>
*Đặc điểm:* 
* Dùng để bảo vệ tài nguyên chia sẻ, chỉ duy nhất 1 task có quyền truy cập tại 1 thời điểm nhất định
* Có tính năng **ưu tiên** (Priority Inversion Handling) - nếu một task có mức ưu tiên cao hơn đang chờ mutex, task giữ mutex có mức ưu tiên thấp hơn sẽ được nâng lên để giải phóng nhanh hơn. 
* Task nào lấy mutex (take) thì phải trả lại mutex (give)
*Ứng dụng:* <br>
* Bảo vệ biến toàn cục, tài nguyên chung (I2C, SPI, UART,...)
* Tránh xung đột khi nhiều task truy cập cùng lúc 
```c
SemaphoreHandle_t xMutex = xSemaphoreCreateMutex();

if(xMutex != NULL){
  if(xSemaphoreTake(xMutex, portMAX_DELAY)){
    //Truy cap tai nguyen an toan 
    xSemaphoreGive(xMutex); //Lay thi phai tra lai mutex luon
  }
}
```
2. `xSemaphoreCreateBinary()` (Binary Semaphore - Semaphore nhị phân) <br>
*Kiểu:* Đồng bộ hóa <br>
*Đặc điểm:* 
* Chỉ có hai trạng thái: có sẵn (1) và không có sẵn (0)
* Không hỗ trợ ưu tiên task như mutex
* Dùng cho đồng bộ hóa (task này báo hiệu task khác)
* Task A có thể "give", Task B có thể "take"
*Ứng dụng:* <br>
* Đồng bộ giữa ISR (Interrupt Service Routine) và task
* Báo hiệu sự kiện giữa 2 task 
```c
SemaphoreHandle_t xBinarySemaphore = xSemaphoreCreateBinary();

if(xBinarySemaphore != NULL){
  xTaskCreate(vTask1, "Task1", 2048, NULL, 2, NULL);

  if(xSemaphoreTake(xBinarySemaphore, portMAX_DELAY)){
    //Chay khi co tin hieu tu task khac
  }
}
```
3. `xSemaphoreCreateCounting()` (Counting semaphore - Semaphore đếm) <br>
\+ *Kiểu:* Đếm tài nguyên <br>
\+ *Đặc điểm*
  * Có thể đếm nhiều hơn 1 task, nghĩa là cho phép nhiều task cùng lúc take và give 
  * Hữu ích khi giới hạn số lượng tài nguyên 
  * Khi tạo cần chỉ định giá trị tối đa (`uxMaxCount`) và giá trị ban đầu (`uxInitialCount`)
\+ *Ứng dụng* <br>
  * Quản lý nhiều tài nguyên giống nhau (ví dụ: 3 cảm biến có thể chạy song song)
  * Kiểm soát số lượng kết nối đồng thời (ví dụ: tối đa 5 client kết nối Wifi)
```c
SemaphoreHandle_t xCountingSemaphore = xSemaphoreCreateCounting(5, 5);

if (xCountingSemaphore != NULL) {
  if (xSemaphoreTake(xCountingSemaphore, portMAX_DELAY)) {
      // Truy cập tài nguyên
      xSemaphoreGive(xCountingSemaphore);
  }
}
```
