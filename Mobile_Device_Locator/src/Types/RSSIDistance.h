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
    if (distance > VERY_CLOSE)
    {
        return "Very Close";
    }

    else if (distance > CLOSE)
    {
        return "Close";
    }

    else if (distance > NEAR)
    {
        return "Near";
    }

    else if (distance > FAR)
    {
        return "Far";
    }

    else
    {
        return "Unknown";
    }
}