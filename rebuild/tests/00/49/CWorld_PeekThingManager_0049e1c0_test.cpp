#include "fable_world.h"
#include <stdio.h>
#include <string.h>
static unsigned long worldStorage[(sizeof(CWorld) + 3) / 4];
static unsigned long managerStorage[4];
int main()
{
    const CWorld* world = reinterpret_cast<const CWorld*>(worldStorage);
    CThingManager* manager = reinterpret_cast<CThingManager*>(managerStorage);
    memcpy(reinterpret_cast<unsigned char*>(worldStorage) + 0x50, &manager, 4);
    if (&world->CWorld::PeekThingManager() != manager) return 1;
    printf("WORLD_PEEK_THING_MANAGER_PASS\n");
    return 0;
}
