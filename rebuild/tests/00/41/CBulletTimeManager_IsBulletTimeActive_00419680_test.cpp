// Behaviour: active when Override (+0x0C) is set or the STLport map's node count
// (+0x04) is non-zero; the Ego_r offsets (+0x10 Override) must not matter.
#include "engine/CBulletTimeManager.h"
#include <stdio.h>
#include <string.h>

static int Run(unsigned char overrideByte, unsigned long count, unsigned char egoOverride)
{
    unsigned char storage[0x20];
    memset(storage, 0, sizeof(storage));
    storage[0x0C] = overrideByte;
    *reinterpret_cast<unsigned long*>(storage + 0x04) = count;
    storage[0x10] = egoOverride;
    return reinterpret_cast<const CBulletTimeManager*>(storage)->IsBulletTimeActive() ? 1 : 0;
}

int main()
{
    if (Run(0, 0, 0) != 0) { printf("FAIL idle\n"); return 1; }
    if (Run(1, 0, 0) != 1) { printf("FAIL override\n"); return 2; }
    if (Run(0, 3, 0) != 1) { printf("FAIL multipliers\n"); return 3; }
    if (Run(1, 5, 0) != 1) { printf("FAIL both\n"); return 4; }
    if (Run(0, 0, 1) != 0) { printf("FAIL ego offset\n"); return 5; }
    printf("PASS_00419680\n");
    return 0;
}