#define LED_PIN 14

void basic_high_low();

void basic_high_low() {
    pinMode(LED_PIN, OUTPUT);
    
    digitalWrite(LED_PIN, HIGH);
    Serial.println("high");
    delay(1000);
    digitalWrite(LED_PIN, LOW);
    Serial.println("low");
    delay(1000);
}