#pragma once
#include "../Types/WSData.h"
#include <ArduinoWebsockets.h>
using namespace websockets;

class WSClient
{
public:
    static void setup();
    static void send(WSData &data);

private:
    // static void onMessage(WebsocketsMessage message);
    static void onEvent(WebsocketsEvent event, String data);
    static WebsocketsClient instance;
};