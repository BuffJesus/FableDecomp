#include <cstring>

struct CGameEvent
{
    long type;
    unsigned char player;
    unsigned char data[0x20];
    unsigned char endPos;
    unsigned char valid;
    unsigned char replacement;
};
struct CGameEventPackage
{
    unsigned long sequence;
    unsigned long eventCount;
    CGameEvent events[40];
};
class CGameEventPackageSet
{
public:
    unsigned long packageCount;
    CGameEventPackage packages[50];
    long CompressIntoBuffer(unsigned char* buffer) const;
};

long CGameEventPackageSet::CompressIntoBuffer(unsigned char* buffer) const
{
    struct EventTail
    {
        unsigned char player;
        unsigned char data[0x20];
        unsigned char endPos;
        unsigned char valid;
        unsigned char replacement;
    };
    unsigned char* out = buffer;
    *out++ = static_cast<unsigned char>(packageCount);
    unsigned long packageIndex = 0;
    if (packageCount > 0)
    {
        const unsigned long* packageEventCount = &packages[0].eventCount;
        do
        {
            *out++ = static_cast<unsigned char>(*packageEventCount);
            *reinterpret_cast<unsigned long*>(out) = packageEventCount[-1];
            out += 4;
            unsigned long eventIndex = 0;
            if (*packageEventCount > 0)
            {
                const EventTail* event = reinterpret_cast<const EventTail*>(
                    reinterpret_cast<const unsigned char*>(packageEventCount) + 8);
                do
                {
                    unsigned short type = *reinterpret_cast<const unsigned short*>(
                        reinterpret_cast<const unsigned char*>(event) - 4);
                    if (event->replacement != 0)
                        type = static_cast<unsigned short>(type | 0x8000);
                    *reinterpret_cast<unsigned short*>(out) = type;
                    out += 2;
                    *out++ = event->player;
                    unsigned char payloadSize = event->endPos;
                    *out++ = payloadSize;
                    const unsigned char* source = &event->data[0];
                    std::memcpy(out, source, payloadSize);
                    out += payloadSize;
                    ++eventIndex;
                    event = reinterpret_cast<const EventTail*>(
                        reinterpret_cast<const unsigned char*>(event) + 0x28);
                }
                while (eventIndex < *packageEventCount);
            }
            ++packageIndex;
            packageEventCount = reinterpret_cast<const unsigned long*>(
                reinterpret_cast<const unsigned char*>(packageEventCount) + 0x648);
        }
        while (packageIndex < packageCount);
    }
    return static_cast<long>(out - buffer);
}