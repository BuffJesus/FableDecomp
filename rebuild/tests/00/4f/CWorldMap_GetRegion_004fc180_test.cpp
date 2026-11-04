// Behaviour: GetRegion(i) returns &Regions._M_start[i] (0x58-byte CRegion stride),
// read from the vector start at retail +0x2C of `this`.
#include "fable_world_map.h"
#include <stdio.h>
#include <string.h>

int main()
{
    static unsigned char storage[0x100];
    static unsigned char regions[0x58 * 4];
    memset(storage, 0, sizeof(storage));
    *reinterpret_cast<unsigned char**>(storage + 0x2C) = regions;
    *reinterpret_cast<unsigned char**>(storage + 0x30) = regions + 0x58 * 4;
    const CWorldMap* map = reinterpret_cast<const CWorldMap*>(storage);
    for (long i = 0; i < 4; ++i)
    {
        const CRegion* r = &map->GetRegion(i);
        if (reinterpret_cast<const unsigned char*>(r) != regions + 0x58 * i) { printf("FAIL index %ld\n", i); return 1; }
    }
    // No bounds check in retail: index 9 is still start + 9 * 0x58.
    if (reinterpret_cast<const unsigned char*>(&map->GetRegion(9)) != regions + 0x58 * 9) { printf("FAIL unchecked\n"); return 2; }
    // The vector itself must be left alone (the fixture storage is static; no dtor runs).
    printf("004fc180_TEST PASS\n");
    return 0;
}