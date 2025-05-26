#include "PSService.h"

void PSService::promiscuousSniffCallback(void *buf, wifi_promiscuous_pkt_type_t type)
{
    if (type == WIFI_PKT_MGMT)
    {
        wifi_promiscuous_pkt_t *pkt = (wifi_promiscuous_pkt_t *)buf;
        if (pkt->rx_ctrl.rssi > -70)
        {
            Serial.println("Fine signal strength detected.");
            Serial.printf("Management frame: %d bytes, RSSI: %d Payload: %u\n", pkt->rx_ctrl.sig_len, pkt->rx_ctrl.rssi, pkt->payload);
            return;
        }
    }
    else if (type == WIFI_PKT_CTRL)
    {
        Serial.println("Control frame detected.");
    }
    else if (type == WIFI_PKT_DATA)
    {
        Serial.println("Data frame detected.");
    }
    else
    {
        Serial.println("Unknown packet type.");
    }
}

void PSService::setPromiscuousMode(bool enable)
{
    if (enable)
    {
        esp_wifi_set_promiscuous_rx_cb(promiscuousSniffCallback);
        esp_wifi_set_promiscuous(true);
    }
    else
    {
        esp_wifi_set_promiscuous(false);
    }
}


void PSService::setup()
{
    WiFi.mode(WIFI_MODE_STA);
    Serial.println("Setting up promiscuous mode...");
    setPromiscuousMode();
    Serial.println("Promiscuous mode set up complete.");

    Serial.println("Registering promiscuous callback...");
    esp_wifi_set_promiscuous_rx_cb(promiscuousSniffCallback);
    Serial.println("Callback registered.");
    wifi_scan_config_t config = {
        .scan_type = WIFI_SCAN_TYPE_ACTIVE,
    };
    esp_wifi_scan_start(&config, true);
}