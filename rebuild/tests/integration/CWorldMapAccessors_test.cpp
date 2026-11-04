#include "fable_world.h"
#include <stdio.h>
#include <string.h>

static unsigned long worldStorage[(sizeof(CWorld) + 3) / 4];
static unsigned long mapStorage[4][16];

int main()
{
    CWorld* world = reinterpret_cast<CWorld*>(worldStorage);
    memset(worldStorage, 0xa5, sizeof(worldStorage));
    int cases = 0;
    for (int selector = 0; selector < 4; ++selector)
    for (int pass = 0; pass < 3; ++pass)
    {
        CWorldMap* map = reinterpret_cast<CWorldMap*>(mapStorage[selector]);
        memcpy(reinterpret_cast<unsigned char*>(worldStorage) + 0x14, &map, 4);
        if (&world->CWorld::GetWorldMap() != map) return 1;
        const CWorld* readonly = world;
        if (&readonly->CWorld::PeekWorldMap() != map) return 2;
        if (memcmp(reinterpret_cast<unsigned char*>(worldStorage) + 0x14, &map, 4)) return 3;
        ++cases;
    }
    printf("WORLD_MAP_ACCESSORS_PASS cases=%d\n", cases);
    return 0;
}
