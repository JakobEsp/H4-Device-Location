#pragma once
#include <Arduino.h>
#include <WiFi.h>

#define WIFI_SSID "IoT_H3/4"
#define WIFI_PASSWORD "98806829"

class NetworkService {
    public: 
        static void connect();
        static void disconnect();
        static bool isConnected();
};