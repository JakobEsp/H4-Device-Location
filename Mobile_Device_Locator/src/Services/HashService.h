#pragma once
#include <bits/stdc++.h>
class HashService
{
public:
    static size_t hashMACAddress(const char *macAddress)
    {
        static std::hash<std::string> hash; // Buffer for the hash, adjust size as needed
        return hash(std::string(macAddress));
    }
};