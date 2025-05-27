#include "WSClient.h"
#include <ArduinoWebsockets.h>
#include "NetworkService.h"
// http://172.22.224.1:3000/_ws
const char ip[] = "172.22.224.1";
const int port = 3000;
const char path[] = "/_ws";
void WSClient::setup()
{
    if (!NetworkService::isConnected()) {
        NetworkService::connect();
    } 
    
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
