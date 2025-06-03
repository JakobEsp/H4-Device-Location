#include "WSClient.h"
#include <ArduinoWebsockets.h>
#include "NetworkService.h"

// ws://172.22.224.1:3000/_ws
const char ip[] = "192.168.0.102";
const int port = 3000;
const char path[] = "/_ws";
WebsocketsClient WSClient::instance;

void WSClient::setup()
{
    if (!NetworkService::isConnected())
    {
        NetworkService::connect();
    }

    instance.onEvent(onEvent);

    if (!instance.connect(ip, port, path))
    {
        Serial.println("Failed to connect to WebSocket server");
        return;
    }

    Serial.println("WebSocket client setup complete");
}

void WSClient::send(WSData &data)
{
    if (!instance.available())
    {
        Serial.println("WebSocket ping failed, attempting to reconnect...");
        delay(1000); // Wait for a second before retrying
        setup();
    }


    Serial.printf("%s\n", data.toJson());
    if (!instance.send(data.toJson()))
    {
        Serial.println("Failed to send data over WebSocket");
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
        setup();
    }
}
