// Behaviour fixture for CBulletTimeManager::FrameUpdate (provider CBulletTimeManager_FrameUpdate_004c5e90.cpp): the
// per-frame update must leave every member of the manager (raw storage laid out like retail) unchanged.
#include <stdio.h>
#include <string.h>
#include "engine/CBulletTimeManager.h"

int main()
{
    unsigned char raw[sizeof(CBulletTimeManager)], before[sizeof(CBulletTimeManager)];
    for (unsigned i = 0; i < sizeof(raw); ++i) raw[i] = (unsigned char)(0x30 + i);
    memcpy(before, raw, sizeof(raw));
    CBulletTimeManager* manager = reinterpret_cast<CBulletTimeManager*>(raw);
    for (int i = 0; i < 16; ++i)
        manager->FrameUpdate();
    if (memcmp(raw, before, sizeof(raw)) != 0) { printf("BULLET_TIME_FRAME_UPDATE_FAIL\n"); return 1; }
    printf("BULLET_TIME_FRAME_UPDATE_OK\n");
    return 0;
}
