#include "fable_world.h"
#include <stdio.h>
#include <string.h>
static unsigned long worldStorage[(sizeof(CWorld) + 3) / 4];
static unsigned long managerStorage[4];
int main()
{
    CWorld* world = reinterpret_cast<CWorld*>(worldStorage);
    CThingManager* manager = reinterpret_cast<CThingManager*>(managerStorage);
    memcpy(reinterpret_cast<unsigned char*>(worldStorage) + 0x50, &manager, 4);
    if (&world->CWorld::GetThingManager() != manager) return 1;
    printf("WORLD_GET_THING_MANAGER_PASS\n");
    return 0;
}
