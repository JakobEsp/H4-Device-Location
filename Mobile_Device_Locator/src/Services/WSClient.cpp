#include "WSClient.h"
#include <ArduinoWebsockets.h>
#include "NetworkService.h"
#include "./PSService.h"

// ws://172.22.224.1:3000/_ws
const char ip[] = "192.168.0.102";
const int port = 3000;
const char path[] = "/_ws";
WebsocketsClient WSClient::instance;
bool WSClient::isConnecting = false;
bool WSClient::needsReconnect = false;

void WSClient::setup()
{
    if (isConnecting)
    {
        Serial.println("WebSocket client is already connecting");
        return;
    }
    isConnecting = true;

    if (!NetworkService::isConnected())
    {
        NetworkService::connect();
    }

    instance = WebsocketsClient();
    instance.onEvent(onEvent);

    if (!instance.connect(ip, port, path))
    {
        Serial.println("Failed to connect to WebSocket server");
        return;
    }

    Serial.println("WebSocket client setup complete");
    isConnecting = false;
    needsReconnect = false;
}

void WSClient::send(WSData &data)
{
    // Serial.printf("Sending data to WebSocket server: %d\n", millis() - PromService::getLastSentPacketTime());
    if (millis() - PromService::getLastSentPacketTime() > 1000)
    {
        needsReconnect = true;
    }
    if (!instance.available(true))
    {
        Serial.println("WebSocket ping failed, attempting to reconnect...");
        needsReconnect = true;
    }

    Serial.printf("%s\n", data.toJson());
    if (!instance.send(data.toJson()))
    {
        Serial.println("Failed to send data over WebSocket");
        needsReconnect = true;
    }
    else
    {
        PromService::setLastSentPacketTime();
    }
}

void WSClient::onEvent(WebsocketsEvent event, String data)
{
    Serial.printf("WebSocket event: %d, Data: %s\n", event, data.c_str());
    if (event == WebsocketsEvent::ConnectionOpened)
    {
        Serial.println("WebSocket connection opened");
    }
    else if (event == WebsocketsEvent::ConnectionClosed)
    {
        Serial.println("WebSocket connection closed");
        needsReconnect = true;
    }
}