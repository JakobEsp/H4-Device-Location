#pragma once

enum RSSIDistance
{
    VERY_CLOSE = -40,
    CLOSE = -55,
    NEAR = -70,
    FAR = -80,
};

inline const char* RSSIDistanceToString(RSSIDistance distance)
{
    switch (distance)
    {
    case VERY_CLOSE:
        return "Very Close";
    case CLOSE:
        return "Close";
    case NEAR:
        return "Near";
    case FAR:
        return "Far";
    default:
        return "Unknown";
    }
}