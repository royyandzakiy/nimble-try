#if !CONFIG_AUTOSTART_ARDUINO

#include "esp_task_wdt.h"
// #include "esp32-hal-bt.h"

extern void setup();
extern void loop();

TaskHandle_t loopTaskHandle = NULL;

bool loopTaskWDTEnabled;

void loopTask(void *pvParameters)
{
    setup();
    for(;;) {
        if(loopTaskWDTEnabled){
            esp_task_wdt_reset();
            vTaskDelay(10 / portTICK_PERIOD_MS);
        }
        loop();
    }
}

extern "C" void app_main()
{
    loopTaskWDTEnabled = true; 
    
    //prevent bluetooth memory release, just call any function from esp32-hal-bt.h
    // btStarted(); // ROY: whilst build, getting error "undefined reference to `btStarted'"

    // initArduino();
    xTaskCreatePinnedToCore(loopTask, "loopTask", 8192, NULL, 1, &loopTaskHandle, 1);
}

#endif