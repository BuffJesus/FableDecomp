#include "fable_game_event.h"
#include <stdio.h>
#include <string.h>

static unsigned long storage[(sizeof(CGameEventPackage) + 3) / 4];
static unsigned char expected[sizeof(CGameEventPackage)];

int main()
{
    CGameEventPackage* package = reinterpret_cast<CGameEventPackage*>(storage);
    const unsigned long counts[] = {0, 1, 2, 39, 40, 0x7fffffffUL, 0x80000000UL, 0xffffffffUL};
    const unsigned long indices[] = {0, 1, 17, 39};
    int cases = 0;
    for (unsigned int i = 0; i < sizeof(counts) / sizeof(counts[0]); ++i)
    {
        memset(storage, 0xa5, sizeof(storage));
        memset(expected, 0xa5, sizeof(expected));
        storage[1] = counts[i];
        memcpy(expected + 4, &counts[i], 4);
        if (package->GetNoEvents() != counts[i] ||
            memcmp(storage, expected, sizeof(expected))) return 1;
        ++cases;
        for (unsigned int j = 0; j < sizeof(indices) / sizeof(indices[0]); ++j)
        {
            const CGameEvent* event = reinterpret_cast<const CGameEvent*>(
                reinterpret_cast<const unsigned char*>(package) + 8 + indices[j] * 0x28);
            if (package->GetEvent(indices[j]) != event ||
                memcmp(storage, expected, sizeof(expected))) return 2;
            ++cases;
        }
        package->Clear();
        memset(expected + 4, 0, 4);
        if (memcmp(storage, expected, sizeof(expected))) return 3;
        ++cases;
    }
    printf("EVENT_PACKAGE_ACCESSORS_PASS cases=%d\n", cases);
    return 0;
}
