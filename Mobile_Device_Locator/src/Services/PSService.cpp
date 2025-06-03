#include "PSService.h"
#include <Types/WSData.h>
#include "WSClient.h"
#include <Types/RSSIDistance.h>
int PromService::channel = 1;
unsigned long PromService::lastSentPacketTime = millis();
void PromService::promiscuousSniffCallback(void *buf, wifi_promiscuous_pkt_type_t type)
{
    if ((millis() - lastSentPacketTime) < 500) {
        return;
    }

    if (type != WIFI_PKT_MGMT)
    {
        return;
    }
    wifi_promiscuous_pkt_t *pkt = (wifi_promiscuous_pkt_t *)buf;
    if (pkt->rx_ctrl.rssi <= RSSIDistance::NEAR)
    {
        return;
    }

    WSData data(pkt);
    WSClient::send(data);
    lastSentPacketTime = millis();
}

void PromService::setPromiscuousMode(bool enable)
{
    esp_wifi_set_promiscuous(enable);
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