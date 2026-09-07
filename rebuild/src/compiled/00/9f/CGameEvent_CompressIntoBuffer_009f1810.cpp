#include <cstring>

class CGameEvent
{
public:
    long type;
    unsigned char player;
    unsigned char data[0x20];
    unsigned char endPos;
    unsigned char valid;
    unsigned char replacement;
    long CompressIntoBuffer(unsigned char* buffer) const;
};

long CGameEvent::CompressIntoBuffer(unsigned char* buffer) const
{
    unsigned short serializedType = static_cast<unsigned short>(type);
    if (replacement != 0)
        serializedType = static_cast<unsigned short>(serializedType | 0x8000);
    unsigned char* out = buffer;
    *reinterpret_cast<unsigned short*>(out) = serializedType;
    out += 2;
    *out++ = player;
    unsigned char payloadSize = endPos;
    *out++ = payloadSize;
    std::memcpy(out, &data[0], payloadSize);
    out += payloadSize;
    return static_cast<long>(out - buffer);
}