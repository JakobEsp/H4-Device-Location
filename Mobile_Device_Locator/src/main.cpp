#include <Arduino.h>
#include "Services/PSService.h"
#include <esp_wifi.h>
#include "Services/WSClient.h"
#include "Services/NetworkService.h"
// put function declarations here:

void setup() {
    Serial.begin(115200); // output serial to the serial monitor in terminal
    Serial.println("Starting PSService...");
    PromService::setup(); 
    NetworkService::connect(); // connect to the network
    WSClient::setup(); // setup the WebSocket client
}

void loop() {
  PromService::incrementChannel();
  delay(1000); // wait for 1 second
  esp_wifi_set_channel(PromService::getChannel(), WIFI_SECOND_CHAN_NONE);
}