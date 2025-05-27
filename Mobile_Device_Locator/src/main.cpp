#include <Arduino.h>
#include "Services/PSService.h"
#include <esp_wifi.h>
#include "Services/WSClient.h"
#include "Services/NetworkService.h"
#include <chrono>
#include "Types/Unix.h"
#include "Services/NTPService.h"
// put function declarations here:

void setup()
{
  Serial.begin(115200); // output serial to the serial monitor in terminal
  NetworkService::connect(); // connect to the network
  NTPService::setup(); // setup the NTP service to get the current time
  WSClient::setup();         // setup the WebSocket client
  PromService::setup();
}

void loop()
{
  PromService::incrementChannel();
  delay(1000); // wait for 1 second
  esp_wifi_set_channel(PromService::getChannel(), WIFI_SECOND_CHAN_NONE);
}