#include "fable_world.h"
#include <stdio.h>
#include <string.h>

static unsigned long worldStorage[(sizeof(CWorld) + 3) / 4];
static unsigned long managerStorage[4][16];

int main()
{
    CWorld* world = reinterpret_cast<CWorld*>(worldStorage);
    memset(worldStorage, 0xa5, sizeof(worldStorage));
    int cases = 0;
    for (int selector = 0; selector < 4; ++selector)
    for (int pass = 0; pass < 3; ++pass)
    {
        CThingManager* manager = reinterpret_cast<CThingManager*>(managerStorage[selector]);
        memcpy(reinterpret_cast<unsigned char*>(worldStorage) + 0x50, &manager, 4);
        if (&world->CWorld::GetThingManager() != manager) return 1;
        const CWorld* readonly = world;
        if (&readonly->CWorld::PeekThingManager() != manager) return 2;
        if (memcmp(reinterpret_cast<unsigned char*>(worldStorage) + 0x50, &manager, 4)) return 3;
        ++cases;
    }
    printf("WORLD_THING_MANAGER_ACCESSORS_PASS cases=%d\n", cases);
    return 0;
}
