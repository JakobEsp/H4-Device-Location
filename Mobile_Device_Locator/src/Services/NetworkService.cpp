#include  "NetworkService.h"


void NetworkService::connect() {
    WiFi.mode(WIFI_MODE_STA);
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.println("Connecting to WiFi...");
    }

    Serial.println("Connected to WiFi");
}

void NetworkService::disconnect() {
    if (WiFi.status() == WL_CONNECTED) {
        WiFi.disconnect();
        Serial.println("Disconnected from WiFi");
    } else {
        Serial.println("Not connected to any WiFi network");
    }
}

wl_status_t NetworkService::status() {
    wl_status_t status = WiFi.status();
    return status; // Return the status for further processing if needed
}