#include <Arduino.h>
#include "Services/PSService.h"
#include <esp_wifi.h>
#include "Services/WSClient.h"
#include "Services/NetworkService.h"
#include <chrono>
#include "Services/NTPService.h"
// put function declarations here:

void setup()
{
  Serial.begin(115200);      // output serial to the serial monitor in terminal
  NetworkService::connect(); // connect to the network
  NTPService::setup();       // setup the NTP service to get the current time
  WSClient::setup();         // setup the WebSocket client
  PromService::setup();
}

void loop()
{
  PromService::incrementChannel();
  delay(2500); // wait for 1 second
  int channel = PromService::getChannel();
  WSClient::setup();
  PromService::setup();
  esp_err_t error = esp_wifi_set_channel(channel, WIFI_SECOND_CHAN_NONE);
  if (error == ESP_OK)
  {
    Serial.printf("Channel set to: %d\n", PromService::getChannel());
  }
}