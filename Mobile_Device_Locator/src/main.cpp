#include <Arduino.h>
#include "Services/PSService.h"
#include <esp_wifi.h>
// put function declarations here:

void setup() {
    Serial.begin(115200); // output serial to the serial monitor in terminal
    Serial.println("Starting PSService...");
    PSService::setup(); 
}

void loop() {
  // put your main code here, to run repeatedly:
}