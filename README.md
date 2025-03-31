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

4. `pdMS_TO_TICKS()` (Chuyển đổi từ milliseconds sang tick)
Nếu bạn muốn viết code dễ đọc hơn, thay vì dùng `/ portTICK_PERIOD_MS` thì có thể dùng hàm trên
```c 
vTaskDelay(pdMS_TO_TICKS(1000)); //Delay 1000ms (1s)
```

