#include "NetworkService.h"

void NetworkService::connect()
{
    WiFi.mode(WIFI_MODE_STA);
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    Serial.print("\nConnecting to WiFi...");
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(1000);
        Serial.print(".");
    }

    Serial.println("\nConnected to WiFi");
}

void NetworkService::disconnect()
{
    if (WiFi.status() == WL_CONNECTED)
    {
        WiFi.disconnect();
        Serial.println("Disconnected from WiFi");
    }
    else
    {
        Serial.println("Not connected to any WiFi network");
    }
}

bool NetworkService::isConnected()
{
    return WiFi.status() == wl_status_t::WL_CONNECTED; // Return the status for further processing if needed
}