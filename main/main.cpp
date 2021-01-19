#include <Arduino.h>

void setup() {
    Serial.begin(115200);
    Serial.println("espidf-arduino-bareminimum");
}

void loop() {
    // nothing...
}

extern "C" void app_main() {
    setup();
    while(1) loop();
}