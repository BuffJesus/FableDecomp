#include "fable_world.h"
#include <stdio.h>
#include <string.h>
static unsigned long worldStorage[(sizeof(CWorld) + 3) / 4];
static unsigned long mapStorage[4];
int main()
{
    CWorld* world = reinterpret_cast<CWorld*>(worldStorage);
    CWorldMap* map = reinterpret_cast<CWorldMap*>(mapStorage);
    memcpy(reinterpret_cast<unsigned char*>(worldStorage) + 0x14, &map, 4);
    if (&world->CWorld::GetWorldMap() != map) return 1;
    printf("WORLD_GET_MAP_PASS\n");
    return 0;
}
