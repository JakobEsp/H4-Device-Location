#include "PSService.h"
int PromService::channel = 1;
void PromService::promiscuousSniffCallback(void *buf, wifi_promiscuous_pkt_type_t type)
{
    if (type != WIFI_PKT_MGMT)
    {
        return;
    }
    wifi_promiscuous_pkt_t *pkt = (wifi_promiscuous_pkt_t *)buf;

    // if (pkt->rx_ctrl.rssi <= -70)
    // {
    //     return;
    // }

    // Serial.printf("Management frame: %d bytes, RSSI: %d Payload: %u\n",
    //               pkt->rx_ctrl.sig_len, pkt->rx_ctrl.rssi, pkt->payload);

    uint8_t *payload = pkt->payload;

    // The source MAC address is at byte offset 10 (802.11 header):
    uint8_t *src_mac = &payload[10];
    // The source MAC address is at byte offset 10 (802.11 header):
    char macStr[18];
    snprintf(macStr, sizeof(macStr), "%02X:%02X:%02X:%02X:%02X:%02X",
             src_mac[0], src_mac[1], src_mac[2],
             src_mac[3], src_mac[4], src_mac[5]);
    if (strstr(macStr, "44") != nullptr)
        return;
    printf("Channel: %d Source MAC: %s\n", pkt->rx_ctrl.channel, macStr);

    Serial.println();
}

void PromService::setPromiscuousMode(bool enable)
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

void PromService::setup()
{
    WiFi.mode(WIFI_MODE_STA);
    setPromiscuousMode();

    esp_wifi_set_promiscuous_rx_cb(promiscuousSniffCallback);
    Serial.println("Promiscuous setup done!");
    esp_wifi_start();
}

int PromService::getChannel()
{
    return PromService::channel;
}

void PromService::incrementChannel()
{
    PromService::channel = (PromService::channel + 1) % 14;
    if (PromService::channel == 0)
        PromService::channel = 1;
}