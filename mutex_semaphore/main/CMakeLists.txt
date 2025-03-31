idf_component_register(SRCS "mutex_semaphore.c"
                      INCLUDE_DIRS "."
                      REQUIRES esp_timer freertos driver)