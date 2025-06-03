#pragma once

#include <cstdint>
#include <esp_wifi_types.h>
#include <string.h>
#include <time.h>
#include <Services/NTPService.h>
#include "RSSIDistance.h"
#include <Services/HashService.h>
#include <Esp.h>

class WSData
{
public:
    WSData(wifi_promiscuous_pkt_t *values)
    {
        this->pkt = values;
        macAddress = new char[18]; // 17 characters + null terminator
        uint8_t *payload = pkt->payload;

        // The source MAC address is at byte offset 10 (802.11 header):
        uint8_t *src_mac = &payload[10];
        snprintf(macAddress, 18, "%02X:%02X:%02X:%02X:%02X:%02X",
                 src_mac[0], src_mac[1], src_mac[2],
                 src_mac[3], src_mac[4], src_mac[5]);
        // this->pkt->rx_ctrl.timestamp = Unix::getTimestamp();
        setHwid();
        
    }

    static void setHwid()
    {
        if (hwid)
        {
            return;
        }
        hwid = ESP.getEfuseMac();
    }

    int getChannel() const {
        return pkt->rx_ctrl.channel;
    }

    const char *convertTime()
    {
        static char timeStr[20];
        struct tm *timeinfo = NTPService::getTime();
        if (!timeinfo)
        {
            return "Time not available";
        }

        uint64_t timestamp = mktime(timeinfo) + (pkt->rx_ctrl.timestamp / 1000000); // Convert microseconds to seconds
        struct tm *tm_info;
        tm_info = localtime((time_t *)&timestamp);
        strftime(timeStr, sizeof(timeStr), "%Y-%m-%d %H:%M:%S", tm_info);
        delete timeinfo; // Clean up the allocated memory for timeinfo
        return timeStr;
    }

    const char *toJson()
    {
        static char json[256];
        if (!pkt)
        {
            return "{\"error\":\"No packet data available\"}";
        }
        // %012llx
        static char c_hwid[32];
        sprintf(c_hwid, "%012llx", hwid);

        sprintf(json,
                "{"
                "\"macAddress\":%llu,"
                "\"channel\":%d,"
                "\"rssi\":%d,"
                "\"rssi_type\": \"%s\","
                "\"sig_len\":%d,"
                "\"timestamp\":%llu,"
                "\"time\":\"%s\","
                "\"hwid\":\"%s\""
                "}",
                (unsigned long long)HashService::hashMACAddress(macAddress),
                pkt->rx_ctrl.channel,
                pkt->rx_ctrl.rssi,
                RSSIDistanceToString((RSSIDistance)pkt->rx_ctrl.rssi),
                pkt->rx_ctrl.sig_len,
                (unsigned long long)pkt->rx_ctrl.timestamp,
                convertTime(),
                c_hwid);
        return json;
    }

    ~WSData()
    {
        delete[] macAddress;
        macAddress = nullptr;
        pkt = nullptr;
    }

private:
    char *macAddress;
    wifi_promiscuous_pkt_t *pkt;
    static uint64_t hwid;
};