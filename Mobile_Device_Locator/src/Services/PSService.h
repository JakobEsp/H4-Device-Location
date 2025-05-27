#pragma once
#include <esp_wifi_types.h>
#include <Arduino.h>
#include <WiFi.h>
#include <esp_wifi.h>

class PSService {
    public:
        static void promiscuousSniffCallback(void *buf, wifi_promiscuous_pkt_type_t type);
        static void setup();
        static void incrementChannel();
        
        static int getChannel();
        private:
            static void setPromiscuousMode(bool enable = true);
            static int channel; 
};