#include "fable_game.h"
#include <stdio.h>
#include <string.h>

static unsigned long storage[(sizeof(CGameEventPackageSet) + 3) / 4];
static unsigned char snapshot[sizeof(CGameEventPackageSet)];
static unsigned char output[100000];
static unsigned char expected[100000];

static void Write32(unsigned char* place, unsigned long value)
{
    memcpy(place, &value, 4);
}

int main()
{
    const unsigned int packages[] = {0, 1, 2, 5, 50};
    const unsigned int events[] = {0, 1, 3, 40};
    int cases = 0;
    for (unsigned int pi = 0; pi < 5; ++pi)
    for (unsigned int ei = 0; ei < 4; ++ei)
    {
        memset(storage, 0xa5, sizeof(storage));
        memset(output, 0xcc, sizeof(output));
        memset(expected, 0xcc, sizeof(expected));
        Write32(reinterpret_cast<unsigned char*>(storage), packages[pi]);
        unsigned int pos = 0;
        expected[pos++] = static_cast<unsigned char>(packages[pi]);
        for (unsigned int p = 0; p < packages[pi]; ++p)
        {
            unsigned char* package = reinterpret_cast<unsigned char*>(storage) + 4 + p * 0x648;
            unsigned long stamp = 0x11223344UL + p * 0x10001UL;
            Write32(package, stamp);
            Write32(package + 4, events[ei]);
            expected[pos++] = static_cast<unsigned char>(events[ei]);
            Write32(expected + pos, stamp);
            pos += 4;
            for (unsigned int e = 0; e < events[ei]; ++e)
            {
                unsigned char* event = package + 8 + e * 0x28;
                unsigned long type = 0x12340000UL | ((p * 53 + e * 7 + 0x111) & 0x7fff);
                Write32(event, type);
                event[4] = static_cast<unsigned char>(p * 17 + e * 3 + 0x80);
                event[0x25] = static_cast<unsigned char>((p + e) % 33);
                event[0x27] = static_cast<unsigned char>((p + e) & 1);
                for (unsigned int j = 0; j < 32; ++j)
                    event[5 + j] = static_cast<unsigned char>(p * 23 + e * 11 + j * 5);
                unsigned short wireType = static_cast<unsigned short>(type);
                if (event[0x27]) wireType |= 0x8000;
                memcpy(expected + pos, &wireType, 2);
                pos += 2;
                expected[pos++] = event[4];
                expected[pos++] = event[0x25];
                memcpy(expected + pos, event + 5, event[0x25]);
                pos += event[0x25];
            }
        }
        memcpy(snapshot, storage, sizeof(snapshot));
        const CGameEventPackageSet* set = reinterpret_cast<const CGameEventPackageSet*>(storage);
        if (set->CompressIntoBuffer(output) != static_cast<long>(pos)) return 1;
        if (memcmp(output, expected, sizeof(output))) return 2;
        if (memcmp(storage, snapshot, sizeof(snapshot))) return 3;
        ++cases;
    }
    printf("PACKAGE_SET_COMPRESS_PASS cases=%d\n", cases);
    return 0;
}
