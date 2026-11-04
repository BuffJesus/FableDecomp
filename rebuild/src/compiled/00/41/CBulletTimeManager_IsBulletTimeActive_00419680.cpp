// CBulletTimeManager::IsBulletTimeActive, retail 0x00419680
// (FableWin ?IsBulletTimeActive@CBulletTimeManager@@QBE_NXZ): bullet time is on
// while it is forced (Override) or any slowdown multiplier is registered.
#include "engine/CBulletTimeManager.h"

bool CBulletTimeManager::IsBulletTimeActive() const
{
    return Override || !BulletTimeInfoMap.empty();
}