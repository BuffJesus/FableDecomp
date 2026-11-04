#include "fable_game_event.h"
#include <stdio.h>
#include <string.h>

static unsigned long storage[(sizeof(CGameEventPackage) + 3) / 4];
static unsigned char expected[sizeof(CGameEventPackage)];
static unsigned long eventWords[sizeof(CGameEvent) / 4];

int main()
{
    const unsigned long counts[] = {0, 1, 3, 39};
    int cases = 0;
    for (unsigned int ci = 0; ci < 4; ++ci)
    for (unsigned int mode = 0; mode < 6; ++mode)
    for (unsigned int pattern = 0; pattern < 4; ++pattern)
    {
        const unsigned long count = counts[ci];
        memset(storage, 0xa5, sizeof(storage));
        memset(expected, 0xa5, sizeof(expected));
        CGameEventPackage* package = reinterpret_cast<CGameEventPackage*>(storage);
        storage[1] = count;
        memcpy(expected + 4, &count, 4);
        for (unsigned long i = 0; i < count; ++i)
        {
            unsigned char* slot = reinterpret_cast<unsigned char*>(storage) + 8 + i * 0x28;
            for (unsigned int j = 0; j < 0x28; ++j)
                slot[j] = static_cast<unsigned char>(i * 29 + j * 7 + pattern * 11);
            reinterpret_cast<CGameEvent*>(slot)->Type = static_cast<long>(100 + i * 3);
            reinterpret_cast<CGameEvent*>(slot)->Player = static_cast<signed char>(-3 + (i % 5));
            if (mode == 5 && i == 1)
            {
                reinterpret_cast<CGameEvent*>(slot)->Type = 100;
                reinterpret_cast<CGameEvent*>(slot)->Player = -3;
            }
            memcpy(expected + 8 + i * 0x28, slot, 0x28);
        }
        unsigned char* incoming = reinterpret_cast<unsigned char*>(eventWords);
        for (unsigned int j = 0; j < 0x28; ++j)
            incoming[j] = static_cast<unsigned char>(j * 17 + pattern * 31 + mode * 9);
        CGameEvent* event = reinterpret_cast<CGameEvent*>(eventWords);
        event->Replacement = mode != 0;
        unsigned long match = mode == 2 && count ? count - 1 : 0;
        if (count)
        {
            const CGameEvent* target = reinterpret_cast<const CGameEvent*>(
                reinterpret_cast<const unsigned char*>(storage) + 8 + match * 0x28);
            event->Type = target->Type;
            event->Player = target->Player;
        }
        else
        {
            event->Type = 100;
            event->Player = -3;
        }
        if (mode == 3) event->Type = -12345;
        if (mode == 4) event->Player = 97;
        const bool replace = count && (mode == 1 || mode == 2 || mode == 5);
        const unsigned long destination = replace ? match : count;
        memcpy(expected + 8 + destination * 0x28, incoming, 0x28);
        if (!replace)
        {
            const unsigned long next = count + 1;
            memcpy(expected + 4, &next, 4);
        }
        package->AddEvent(event);
        if (memcmp(storage, expected, sizeof(expected))) return 1;
        ++cases;
    }
    printf("EVENT_PACKAGE_ADD_PASS cases=%d\n", cases);
    return 0;
}
