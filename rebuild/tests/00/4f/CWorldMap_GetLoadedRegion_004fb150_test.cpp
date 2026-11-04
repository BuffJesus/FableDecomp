// Behaviour: GetLoadedRegion reads the long at retail +0x9C of `this`, nothing else.
#include "fable_world_map.h"
#include <stdio.h>
#include <string.h>

int main()
{
    static unsigned char storage[0x100];
    memset(storage, 0xA5, sizeof(storage));
    *reinterpret_cast<long*>(storage + 0x9C) = 42;
    *reinterpret_cast<long*>(storage + 0xBC) = 7;   // Ego_r's offset must not be used
    const CWorldMap* map = reinterpret_cast<const CWorldMap*>(storage);
    if (map->GetLoadedRegion() != 42) { printf("FAIL 42\n"); return 1; }
    *reinterpret_cast<long*>(storage + 0x9C) = -1;
    if (map->GetLoadedRegion() != -1) { printf("FAIL -1\n"); return 2; }
    printf("AUTO_TINY_004fb150_TEST PASS\n");
    return 0;
}