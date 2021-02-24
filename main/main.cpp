#include <Arduino.h>
#include "basic_test.h"

void setup() {
    Serial.begin(115200);
    Serial.println("espidf-arduino-bareminimum");
}

void loop() {
    basic_high_low();
}