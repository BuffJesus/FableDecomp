#include "fable_game.h"
#include <cstring>

long CGameEventPackageSet::CompressIntoBuffer(unsigned char* buffer) const
{
    // The retail loop walks from each event's +4 byte. This tail view keeps
    // the same packed 0x28 stride and byte-copy sequence as the compiler.
    struct EventTail
    {
        unsigned char Player;
        unsigned char Data[0x20];
        unsigned char EndPos;
        unsigned char valid;
        unsigned char Replacement;
    };
    unsigned char* out = buffer;
    *out++ = static_cast<unsigned char>(NoPackages);
    unsigned long packageIndex = 0;
    if (NoPackages > 0)
    {
        const unsigned long* packageEventCount = &Packages[0].NoEvents;
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
                    if (event->Replacement != 0)
                        type = static_cast<unsigned short>(type | 0x8000);
                    *reinterpret_cast<unsigned short*>(out) = type;
                    out += 2;
                    *out++ = event->Player;
                    unsigned char payloadSize = event->EndPos;
                    *out++ = payloadSize;
                    const unsigned char* source = &event->Data[0];
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
        while (packageIndex < NoPackages);
    }
    return static_cast<long>(out - buffer);
}
