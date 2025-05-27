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

  PSService::incrementChannel();
  delay(1000); // wait for 1 second
  esp_wifi_set_channel(PSService::getChannel(), WIFI_SECOND_CHAN_NONE);
}