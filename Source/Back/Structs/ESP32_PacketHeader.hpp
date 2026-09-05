#ifndef ESP32_PACKETHEADER_H
#define ESP32_PACKETHEADER_H

#include <cstdint>

struct ESP32_PacketHeader
{
    uint32_t frameId;
    uint32_t frameSize;
    uint16_t packetIndex;
    uint16_t packetCount;
    uint16_t payloadSize;
};

#endif // ESP32_PACKETHEADER_H
