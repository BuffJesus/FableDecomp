// CBulletTimeManager::FrameUpdate (retail 0x004C5E90, Ego_r 0x004C89E0): empty in the shipped build - CWorld::UpdateFrame
// calls it each world frame, but nothing in the bullet-time state needs a per-frame step.
#include "engine/CBulletTimeManager.h"

void CBulletTimeManager::FrameUpdate()
{
}
