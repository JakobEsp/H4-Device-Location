#include "WSClient.h"
#include <ArduinoWebsockets.h>

const char ip[] = "";
const int port = 0;
const char path[] = "/";
void WSClient::setup()
{
    instance.onEvent(onEvent);
    
    if (instance.connect(ip, port, path)) {
        Serial.println("Connected to WebSocket server");
    } else {
        Serial.println("Failed to connect to WebSocket server");
    }
}

void WSClient::send(WSData &data)
{
    instance.send(data.toJson());
}

void WSClient::onEvent(WebsocketsEvent event, String data)
{
    if (event == WebsocketsEvent::ConnectionOpened) {
        Serial.println("WebSocket connection opened");
    } else if (event == WebsocketsEvent::ConnectionClosed) {
        Serial.println("WebSocket connection closed");
    }
}
