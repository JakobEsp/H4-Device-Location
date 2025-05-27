#include "PSService.h"
#include <Types/WSData.h>
#include "WSClient.h"
#include <Types/RSSIDistance.h>
int PromService::channel = 1;
void PromService::promiscuousSniffCallback(void *buf, wifi_promiscuous_pkt_type_t type)
{
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
    Serial.printf("%s\n", data.toJson());
    WSClient::send(data);
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