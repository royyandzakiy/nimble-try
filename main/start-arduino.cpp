#if !CONFIG_AUTOSTART_ARDUINO

#include <Arduino.h>
#include "esp_task_wdt.h"
#include "esp32-hal-bt.h"

TaskHandle_t loopTaskHandle = NULL;

bool loopTaskWDTEnabled;

void loopTask(void *pvParameters)
{
    setup();
    for(;;) {
        if(loopTaskWDTEnabled){
            esp_task_wdt_reset();
        }
        loop();
        if (serialEventRun) serialEventRun();
    }
}

extern "C" void app_main()
{
    loopTaskWDTEnabled = false; 
    
    //prevent bluetooth memory release, just call any function from esp32-hal-bt.h
    // btStarted(); // ROY: whilst build, getting error "undefined reference to `btStarted'"

    initArduino();
    xTaskCreateUniversal(loopTask, "loopTask", 8192, NULL, 1, &loopTaskHandle, CONFIG_ARDUINO_RUNNING_CORE);
}

#endif